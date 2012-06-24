#ifndef GCODEVIEW_H
#define GCODEVIEW_H

#include <QtGui/QApplication>
#include <QGLWidget>

#include "gcode.h"
#include "arcball.h"

#include "mgl/regioner.h"

class GcodeView : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
    void animationUpdate();

public:
    GcodeView(QWidget *parent);

    void loadSliceData(const mgl::Tomograph &tomograph, const  mgl::Regions &regions, const std::vector<mgl::SliceData> &sliceData);

    void loadModel(QString filename);
    void exportModel (QString filename, void *progress);



    bool hasModel();

    void setupViewport(int width, int height);
    void resetView();
    void zoom(float amount);
    void panX(float amount);
    void panY(float amount);
    void setMaximumVisibleLayer(int layer);
    void setMinimumVisibleLayer(int layer);

    void toggleRoofs(bool v);
    void toggleFLoors(bool v);
    void toggleLoops(bool v);
    void toggleSurfs(bool v);
    void toggleInfills(bool v);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void paintGLgcode();

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
    float pan_x;
    float pan_y;

    int maxVisibleLayer;
    int minVisibleLayer;
};

#endif // GCODEVIEW_H
