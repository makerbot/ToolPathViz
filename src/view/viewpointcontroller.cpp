#include "viewpointcontroller.h"
#include <QDebug>
#include <QtOpenGL>

ViewpointController::ViewpointController(ViewModel &view, QObject *parent) :
    QObject(parent),
    m_viewModel(view)
{}

AzimuthZenithController::AzimuthZenithController(ViewModel& view, QObject *parent) :
    ViewpointController(view, parent),
    m_azimuth(s_defaultAzimuth), m_zenith(s_defaultZenith),
    m_targetAzimuth(s_defaultAzimuth), m_targetZenith(s_defaultZenith),
    m_azimuthAnimation(this, "azimuth"),
    m_zenithAnimation(this, "zenith"),
    m_zoomAnimation(this, "zoom"),
    m_dragStart(),
    m_isDragging(false)
{
    m_azimuthAnimation.setDuration(200);
    m_zenithAnimation.setDuration(200);
    m_zoomAnimation.setDuration(200);

    m_targetZoom = zoom();

    updateRotation();
}

void AzimuthZenithController::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_dragStart = event->scenePos();
}

void AzimuthZenithController::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if((not m_isDragging) &&
       (event->buttons().testFlag(Qt::LeftButton)) &&
       (m_dragStart - event->scenePos()).manhattanLength() > 3) {
        qDebug() << (m_dragStart - event->scenePos()).manhattanLength();
        m_isDragging = true;
    }

    if(not m_isDragging)
        return;

    m_azimuthAnimation.stop();
    m_zenithAnimation.stop();

    // for now, we're just gonna calculate current and
    // last to emulate the events we used to get.
    QPointF current = event->scenePos();
    QPointF last = event->lastScenePos();

    setAzimuth(azimuth() + (float)(current.x() - last.x())/2.0);
    setZenith(zenith() - (float)(current.y() - last.y())/2.0);

    m_targetAzimuth = m_azimuth;
    m_targetZenith = m_zenith;

    event->accept();
}

void AzimuthZenithController::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_isDragging = false;
}

void AzimuthZenithController::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    m_zoomAnimation.stop();

    float z = zoom() + (event->delta() / 1200.0f);
    setZoom(z);

    m_targetZoom = zoom();

    event->accept();
}

void AzimuthZenithController::adjustAzimuth(float by) {
    m_targetAzimuth += by;

    m_azimuthAnimation.setEndValue(m_targetAzimuth);
    m_azimuthAnimation.start();
}

void AzimuthZenithController::adjustZenith(float by) {
    m_targetZenith += by;

    m_zenithAnimation.setEndValue(m_targetZenith);
    m_zenithAnimation.start();
}

void AzimuthZenithController::adjustZoom(float by) {
    m_targetZoom += by;

    if (m_targetZoom > s_maxZoom)
        m_targetZoom = s_maxZoom;
    if (m_targetZoom < s_minZoom)
        m_targetZoom = s_minZoom;

    m_zoomAnimation.setEndValue(m_targetZoom);
    m_zoomAnimation.start();
}

float AzimuthZenithController::azimuth() {
    return m_azimuth;
}

float AzimuthZenithController::zenith() {
    return m_zenith;
}

float AzimuthZenithController::zoom() {
    return m_viewModel.zoom();
}

// apply to the transformation node
void AzimuthZenithController::updateRotation() {

    QQuaternion azQ = QQuaternion::fromAxisAndAngle(0,0,1,m_azimuth);
    QQuaternion zQ = QQuaternion::fromAxisAndAngle(1,0,0,m_zenith);

    m_viewModel.setRotation(zQ*azQ);
}

void AzimuthZenithController::setZenith(float zenith) {
    m_zenith = zenith;

    updateRotation();
}
void AzimuthZenithController::setAzimuth(float azimuth) {
    m_azimuth = azimuth;

    updateRotation();
}
void AzimuthZenithController::setZoom(float zoom) {
    if(zoom < s_minZoom)
        zoom = s_minZoom;
    if(zoom > s_maxZoom)
        zoom = s_maxZoom;

    m_viewModel.setZoom(zoom);
}
