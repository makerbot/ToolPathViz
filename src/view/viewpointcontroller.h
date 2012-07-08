#ifndef VIEWPOINTCONTROLLER_H
#define VIEWPOINTCONTROLLER_H

#include <QObject>

#include "viewmodel.h"

/** Abstract base class for things that take user
  * input and turn it into operations on a ViewModel. */
class ViewpointController : public QObject
{
    Q_OBJECT
protected:
    /** The ViewModel this Controller controls */
    ViewModel& m_viewModel;

public:
    /**
      *\param view The ViewModel this Controller will control
      */
    ViewpointController(ViewModel& view, QObject *parent = 0);

    virtual void mousePressEvent( QGraphicsSceneMouseEvent* /*event*/ ) {}
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* /*event*/ ) {}
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* /*event*/ ) {}
    virtual void wheelEvent( QGraphicsSceneWheelEvent* /*event*/ ) {}
};

/** The AzimuthZenith controller uses a spherical coordinate system to
  * control the camera position. (see:
  * http://en.wikipedia.org/wiki/Spherical_coordinate_system).
  *
  * Rotation about the scene is stored as a rotation about the Z axis
  * and an elevation from that axis. We assume a constant radius.
  */
class AzimuthZenithController : public ViewpointController {
    Q_OBJECT
    Q_PROPERTY(float azimuth READ azimuth WRITE setAzimuth)
    Q_PROPERTY(float zenith READ zenith WRITE setZenith)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom)
private:

    static const float s_defaultAzimuth = 168;
    static const float s_defaultZenith = -17;
    static const float s_minZoom = 0.25;
    static const float s_maxZoom = 20;
    static const float s_homeZoom = 0.4;

    float m_azimuth;
    float m_zenith;

    //for animation
    float m_targetAzimuth;
    float m_targetZenith;
    float m_targetZoom;
    QPropertyAnimation m_azimuthAnimation;
    QPropertyAnimation m_zenithAnimation;
    QPropertyAnimation m_zoomAnimation;

    // apply to the transformation node
    void updateRotation();

    // these add to the target and start the animation
    void adjustAzimuth(float by);
    void adjustZenith(float by);
    void adjustZoom(float by);

public:
    AzimuthZenithController(ViewModel &view, QObject *parent = 0);

    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
    void wheelEvent( QGraphicsSceneWheelEvent *event );

    float azimuth();
    void setAzimuth( float azimuth );
    float zenith();
    void setZenith( float zenith );
    float zoom();
    void setZoom( float zoom );

};

// Stub: arcball controller

#endif // VIEWPOINTCONTROLLER_H
