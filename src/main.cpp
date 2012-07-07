#include <QtGui/QApplication>
#include <QMainWindow>

#include "load/parsers.h"
#include "view/toolpathscene.h"

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mw;
    ToolpathScene ts;
    mw.setCentralWidget(new GraphicsView(&ts, &mw));
    mw.show();

    // create the file filter for the QFileDialog from the names of the parsers
    QString filter;
    foreach(QString f, ts.parsers())
    {
        filter.append(f);
        filter.append(";;");
    }

    QFileDialog fileDialog(&mw, QString("Open Toolpath"), QString(""), filter);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    // let the user choose a file
    if(fileDialog.exec() == QFileDialog::Accepted)
    {
        QFileInfo selectedFile(fileDialog.selectedFiles().at(0));
        QString selectedFilter(fileDialog.selectedFilter());

        ts.setParser(selectedFilter);
        ts.setFile(selectedFile);
    }

    // let the user choose a visualizer
    //???

    return a.exec();
}
