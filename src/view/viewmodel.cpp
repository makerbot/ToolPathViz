#include "viewmodel.h"

GLfloat LightAmbient[]= { 0.7f, 0.3f, 0.3f, 1.0f };
GLfloat LightDiffuse[]= { 0.6f, 0.7f, 1.0f, 1.0f };
GLfloat LightPosition[]= { -215.0f, 300.0f, 70.0f, 1.0f };

QVector3D lookAtEye(0.0,300.0f,0.0f);
QVector3D lookAtCenter(0.0f,0.0f,0.0f);
QVector3D lookAtUp(0.0f, 0.0f, 1.0f);

/// transforms the specified object coordinates into window coordinates
QVector3D legacyProject(QRect view, QMatrix4x4 modelMat,
                        QMatrix4x4 projMat, QVector3D obj)
{
    QVector3D vp = projMat.map(modelMat.map(QVector4D(obj,1.0))).toVector3DAffine();
    vp *= 0.5;
    vp += QVector3D(0.5,0.5,0.5);
    vp.setX(vp.x()*view.width() + view.x());
    vp.setY(vp.y()*view.height() + view.y());
    return vp;
}

/// maps the specified window coordinates into object coordinates
QVector3D legacyUnProject(QRect view, QMatrix4x4 modelMat,
                          QMatrix4x4 projMat, QVector3D v)
{
    v.setX( (v.x() - view.x())/view.width());
    v.setY( (v.y() - view.y())/view.height());

    v.setX( v.x()*2 - 1);
    v.setY( v.y()*2 - 1);
    v.setZ( v.z()*2 - 1);

    QMatrix4x4 mmp = modelMat.inverted();
    QMatrix4x4 pmp = projMat.inverted();
    QVector4D vin( v,1.0);
    return mmp.map(pmp.map(vin)).toVector3DAffine();
}

/// TODO: remove these legacy functions and replace with nicer versions.
/// replacement for gluPickMatrix
void legacyPickMatrix(GLdouble x, GLdouble y,
                      GLdouble deltax, GLdouble deltay,
                      GLint viewport[4])
{
    glTranslatef((viewport[2] - 2 * (x - viewport[0])) / deltax,
                 (viewport[3] - 2 * (y - viewport[1])) / deltay, 0);
    glScalef(viewport[2] / deltax, viewport[3] / deltay, 1.0);
}


// Assumes that up is normalized!
/// replacement for gluLookAt
void legacyLookAt(QVector3D eye, QVector3D center, QVector3D up)
{
    QVector3D forward, side;
    GLfloat m[16];

    forward = center - eye;
    forward.normalize();

    side = QVector3D::crossProduct(forward,up);
    side.normalize();


    m[0] = side.x();
    m[4] = side.y();
    m[8] = side.z();

    m[1] = up.x();
    m[5] = up.y();
    m[9] = up.z();

    m[2] = -forward.x();
    m[6] = -forward.y();
    m[10] = -forward.z();

    m[3] = m[7] = m[11] = 0;
    m[12] = m[13] = m[14] = 0;
    m[15] = 1;

    glMultMatrixf(m);
    glTranslatef(-eye.x(), -eye.y(), -eye.z());
}

ViewModel::ViewModel(QObject *parent) :
    QObject(parent),
    m_zoom(1),
    m_aspect(1.0)
{
}

void ViewModel::setRotation(const QQuaternion& r)
{
    m_rotation = r;
    emit updated();
}


void ViewModel::setZoom(const qreal z)
{
    m_zoom = z;
    emit updated();
}

///  resize the scene
void ViewModel::resize( int w, int h )
{
    m_aspect = ((GLfloat)w) / h;
    updateFrustrum();
}

/// Conveience accessor for the opengl symbolic constant GL_VIEWPORT
QRect ViewModel::viewport()
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return QRect(viewport[0], viewport[1], viewport[2], viewport[3]);
}

/// Conveience accessor for the opengl symbolic constant GL_MODELVIEW_MATRIX
QMatrix4x4 ViewModel::modelviewMatrix()
{
    GLdouble modelMat[16];
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    legacyLookAt(lookAtEye, lookAtCenter, lookAtUp);
    glMultMatrixd(rotationMatrix().constData());
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
    return QMatrix4x4(modelMat).transposed();
}

/// Conveience accessor for the opengl symbolic constant GL_PROJECTION_MATRIX
QMatrix4x4 ViewModel::projectionMatrix()
{
    GLdouble projMat[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projMat);
    return QMatrix4x4(projMat).transposed();
}

void ViewModel::updateFrustrum()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    float factor = 0.1 + 1/m_zoom;
    glFrustum(-m_aspect * factor,
              m_aspect * factor,
              -1.0 * factor,
              1.0 * factor,
              5.0,
              500.0);
}

void ViewModel::setupModelview()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    legacyLookAt(lookAtEye, lookAtCenter, lookAtUp);
    glMultMatrixd(rotationMatrix().constData());
}

void ViewModel::setupView()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glShadeModel( GL_SMOOTH );

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_COLOR_MATERIAL);

#if defined(GL_RESCALE_NORMAL)
    glEnable(GL_RESCALE_NORMAL);
#else
    glEnable(GL_NORMALIZE);
#endif

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    updateFrustrum();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    legacyLookAt(lookAtEye, lookAtCenter, lookAtUp);

    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);

    glMultMatrixd(rotationMatrix().constData());
}

void ViewModel::setupPick()
{
    updateFrustrum();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    legacyLookAt(lookAtEye, lookAtCenter, lookAtUp);

    glMultMatrixd(rotationMatrix().constData());
}

QVector3D ViewModel::screenToWorld(const QPointF screenCoords, const qreal depth)
{
    setupModelview();

    return legacyUnProject(viewport(), modelviewMatrix(), projectionMatrix(),
                           QVector3D(screenCoords.x(),screenCoords.y(),depth));
}

QPointF ViewModel::worldToScreen(const QVector3D worldCoords)
{
    setupModelview();

    QVector3D screenCoords = legacyProject(viewport(), modelviewMatrix(),
                                           projectionMatrix(), worldCoords);

    return QPointF(screenCoords.x(), screenCoords.y());
}

qreal ViewModel::distanceFromScreen(const QVector3D worldCoords)
{
    setupModelview();

    QVector3D screenCoords = legacyProject(viewport(), modelviewMatrix(),
                                           projectionMatrix(), worldCoords);

    return screenCoords.z();
}

QVector3D ViewModel::projectViewToObject(const QPointF coords,
                                         const QVector3D centroid)
{
    setupModelview();

    // invert y
    QPointF screencoords(coords.x(), viewport().width() - coords.y());

    // Project centroid to view window, get Z
    QVector3D v1 = legacyProject(viewport(),
                                 modelviewMatrix(),
                                 projectionMatrix(),
                                 centroid);
    // project coords back to world space
    QVector3D v2 = legacyUnProject(viewport(),
                                   modelviewMatrix(),
                                   projectionMatrix(),
                                   QVector3D(screencoords.x(),
                                             screencoords.y(),
                                             v1.z()));
    return v2;
}
