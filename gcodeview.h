#ifndef GCODEVIEW_H
#define GCODEVIEW_H

#include <QtGui/QApplication>
#include <QGLWidget>

#include <gcode.h>

class GcodeView : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

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
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent event);

public:
    gcodeModel model;
private:
    QPoint lastPos;

    float xRot;
    float yRot;
    float zRot;
    float scale;

    int currentLayer;
};

#endif // GCODEVIEW_H
