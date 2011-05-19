#ifndef GCODEVIEW_H
#define GCODEVIEW_H

#include <QtGui/QApplication>
#include <QGLWidget>

#include <gcode.h>
#include <arcball.h>

class GcodeView : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
    void animationUpdate();

public:
    GcodeView(QWidget *parent);

    void loadModel(QString filename);

    void setupViewport(int width, int height);
    void resetView();

    void setCurrentLayer(int layer);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent event);

public:
    gcodeModel model;
private:
    Arcball arcball;

    // TODO: Must this be dynamic?
    QTimer* animationTimer;

    float scale;

    int currentLayer;
};

#endif // GCODEVIEW_H
