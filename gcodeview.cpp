#include "gcodeview.h"

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#import <iostream>


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

GcodeView::GcodeView(QWidget *parent)
    : QGLWidget(parent)
{
    resetView();

    currentLayer = 0;

    resizeGL(this->width(),this->height());
}

void GcodeView::resetView() {
    xRot = 0;
    yRot = 0;
    zRot = 0;
    scale = .1;
}

void GcodeView::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GcodeView::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GcodeView::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GcodeView::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GcodeView::setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES
    glOrthof(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#endif
    // TODO: delme
    glMatrixMode(GL_MODELVIEW);
}

void GcodeView::resizeGL(int width, int height)
{
    setupViewport(width, height);

    arcball.set_params( vec2( (float)(width / 2), (float)(height / 2)),
                       (float) 2*height );
}

void GcodeView::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    double widthRatio = (double)width()/(double)height();

    // do something to the projection?
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-widthRatio, widthRatio, -1, 1, 10, 100);
    glTranslatef(0.0, 0.0, -15.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width(), height());

    glPushMatrix();

    glScalef(scale, scale, scale);

    glTranslatef(0.0, 0.0, model.getModelZCenter());
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    mat4 tmp_rot = arcball.rot;
    glMultMatrixf( (float*) &tmp_rot[0][0] );
    glTranslatef(0.0f, 0.0f,  -model.getModelZCenter());

    glLineWidth(2);
    glBegin(GL_LINE_STRIP);


    for (unsigned int i = 1; i < model.points.size(); i++) {
            point a = model.points[i-1];
            point b = model.points[i];

            if (model.map.heightLessThanLayer(currentLayer, a.z)) {
                glColor4f(0,1,1,.15);
            }
            else if (model.map.heightInLayer(currentLayer, a.z)) {
                glColor4f(1,0,1,1);
            }
            else {
                glColor4f(1,1,1,.01);
            }

            glVertex3f(a.x, a.y, a.z); // origin of the line
            glVertex3f(b.x, b.y, b.z); // ending point of the line
    }

    glEnd( );

    glPopMatrix();
}

void GcodeView::loadModel(QString filename) {
    model.loadGCode(filename.toStdString());
    resetView();


    updateGL();
}

void GcodeView::mousePressEvent(QMouseEvent *event)
{
    arcball.mouse_down(event->x(), height() - event->y());
//    lastPos = event->pos();
}

void GcodeView::mouseMoveEvent(QMouseEvent *event)
{
    // TODO: send key states too?
    arcball.mouse_motion(event->x(), height() - event->y());
//    int dx = event->x() - lastPos.x();
//    int dy = event->y() - lastPos.y();

//    if (event->buttons() & Qt::LeftButton) {
//        setXRotation(xRot + 1 * dy);
//        setYRotation(yRot + 1 * dx);
//    } else if (event->buttons() & Qt::RightButton) {
//        setXRotation(xRot - 1 * dy);
//        setYRotation(yRot - 1 * dx);
//    }
//    lastPos = event->pos();

    updateGL();
}

void GcodeView::mouseReleaseEvent(QMouseEvent *event) {
    arcball.mouse_up();
}

void GcodeView::wheelEvent(QWheelEvent *event)
{
    float newScale = scale*(1 + event->delta()/400.0);
    //   if (newScale > 0.01 && newScale < 2) {
    scale = newScale;
    //   }

    updateGL();
}

void GcodeView::mouseDoubleClickEvent(QMouseEvent event) {
    // TODO: We never get here.
    resetView();
    updateGL();
}

void GcodeView::setCurrentLayer(int layer) {
    if (layer < model.map.size()) {
        currentLayer = layer;
        std::cout << "Current layer: " << layer << ", height: " << model.map.getLayerHeight(layer) << std::endl;
        updateGL();
    }
}
