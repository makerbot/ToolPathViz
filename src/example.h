#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "view/toolpathscene.h"
#include "load/visualizers.h"

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QGraphicsScene *scene, QWidget *parent = 0) :
        QGraphicsView(scene, parent)
    {
        QGLFormat glFormat(QGL::SampleBuffers | QGL::AlphaChannel);
        glFormat.setSwapInterval(1); // vsync

        QGLWidget *glWidget = new QGLWidget(glFormat, this);

        setViewport(glWidget);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        QGraphicsView::resizeEvent(event);
        if(scene())
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    }
};

class VisualizerDialog : public QDialog
{
    Q_OBJECT
public:
    VisualizerDialog(QWidget *parent = 0) :
        QDialog(parent)
    {
        setWindowTitle("Select a visualizer");

        QGridLayout *layout = new QGridLayout(this);
        setLayout(layout);

        QListWidget *list = new QListWidget();

        foreach(QString s, visualizerList())
        {
            list->addItem(s);
        }

        connect(list, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(visualizerClicked(QListWidgetItem*)));

        layout->addWidget(list);
    }

public slots:
    void visualizerClicked(QListWidgetItem* item)
    {
        emit visualizerSelected(item->text());
    }

signals:
    void visualizerSelected(QString);
};

#endif // EXAMPLE_H
