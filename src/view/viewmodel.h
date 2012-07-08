#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>
#include <QtOpenGL>

/** The ViewModel manages all the OpenGL camera stuff.
  * It offers functions for rotating & zooming the view of the scene.
  * It also has a variety of convenience functions for converting between
  * world and screen coordinates.
  */
class ViewModel : public QObject
{
    Q_OBJECT
private:
    /// Zoom factor
    GLfloat m_zoom;
    ///
    QQuaternion m_rotation;
    /// Aspect ratio (w/h)
    GLfloat m_aspect;

    /// Conveience accessors for the opengl symbolic constants
    /// GL_VIEWPORT
    QRect viewport();
    /// GL_MODELVIEW_MATRIX
    QMatrix4x4 modelviewMatrix();
    /// GL_PROJECTION_MATRIX
    QMatrix4x4 projectionMatrix();

    /// Updates the viewing frustrum
    void updateFrustrum();

    /// Sets up the modelview matrix looking at the scene.
    /// A convenience method used by many of the other convenience methods
    void setupModelview();


public:
    ViewModel(QObject *parent = 0);

    QQuaternion rotation() const { return m_rotation; }
    QMatrix4x4 rotationMatrix() const {
        QMatrix4x4 matrix;
        matrix.rotate(m_rotation);
        return matrix;
    }
    qreal zoom() const { return m_zoom; }

    /** Sets up the attributes/matrices used when
        rendering the scene to screen */
    void setupView();

    /** Sets up the attributes/matrices used when
        rendering the scene for picking */
    void setupPick();

    /** A convenience method for obtaining a point in world coordinates
        under a specific place on the screen and at a certain depth */
    QVector3D screenToWorld(const QPointF screenCoords, const qreal depth);

    /** A convenience method for mapping a point
        in world coordinates to the screen */
    QPointF worldToScreen(const QVector3D worldCoords);

    /** A convenience method to get the depth of a point in world coordinates */
    qreal distanceFromScreen(const QVector3D worldCoords);

    /** finds the intersection of (a ray through a point on the screen) and
        (a plane perpendicular to the ray, passing through the centroid) */
    QVector3D projectViewToObject(const QPointF coords, const QVector3D center);

public slots:
    /** Set the Rotation of the view to a specific amount */
    void setRotation(const QQuaternion& r);
    /** Set the zoom of the view. */
    void setZoom(const qreal z);

    void resize(const QRectF&);

signals:
    /** Signals that a change requiring redisplay has occured */
    void updated();
};

#endif // VIEWMODEL_H
