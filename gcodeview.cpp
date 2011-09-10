#include "gcodeview.h"

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include <iostream>


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

GcodeView::GcodeView(QWidget *parent) :
    QGLWidget(parent)
{
    resetView();
    resizeGL(this->width(),this->height());

    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animationUpdate()));

    arcball.set_damping(.05);
}

// TODO: This is very likely not thread safe, and this is probably detrimental to the event timer approach.
void GcodeView::animationUpdate() {
    if (arcball.is_spinning) {
        std::cout << "here!" << std::endl;
        arcball.idle();
        *arcball.rot_ptr = *arcball.rot_ptr * arcball.rot_increment;
        updateGL();

        if (!arcball.is_spinning) {
            animationTimer->stop();
        }
    }
}

void GcodeView::resetView() {
    scale = .1;
    pan_x = 0.0;
    pan_y = 0.0;
    currentLayer = 0;

    // TODO: reset arcball
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

    glTranslatef(pan_x, pan_y, model.getModelZCenter());

    mat4 tmp_rot = arcball.rot;
    glMultMatrixf( (float*) &tmp_rot[0][0] );
    glTranslatef(0.0f, 0.0f, -model.getModelZCenter());

    glLineWidth(2);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(GLVertex), &model.vertexes[0].x);
    glColorPointer(4, GL_FLOAT, sizeof(GLVertex), &model.vertexes[0].r);
    glDrawArrays(GL_LINE_STRIP, 0, model.vertexes.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

void GcodeView::loadModel(QString filename) {
    model.loadGCode(filename);
    resetView();
    updateGL();
}

void GcodeView::exportModel(QString filename) {
    model.exportGCode(filename);
}

bool GcodeView::hasModel() {
    return !(model.points.empty());
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

    updateGL();


}

void GcodeView::mouseReleaseEvent(QMouseEvent *event) {
    arcball.mouse_up();

    // Start an animation timer if the arcball is still spinning
    if (arcball.is_spinning) {
        animationTimer->start(10); // 30fps?
    }
}

void GcodeView::wheelEvent(QWheelEvent *event)
{
    float newScale = scale*(1 + event->delta()/400.0);
    scale = newScale;

    updateGL();
}


void GcodeView::zoom(float amount){
    scale = scale * amount;
    updateGL();
}

void GcodeView::panX(float amount){
    pan_x = pan_x + amount;
    updateGL();
}

void GcodeView::panY(float amount){
    pan_y = pan_y + amount;
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
        model.refreshVertexes(currentLayer);
        updateGL();
    }
}
