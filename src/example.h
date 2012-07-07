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

class VisualizerList : public QListWidget
{
    Q_OBJECT
public:
    VisualizerList(QWidget *parent = 0) :
        QListWidget(parent)
    {
        foreach(QString s, visualizerList())
        {
            addItem(s);
        }

        connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(visualizerClicked(QListWidgetItem*)));
    }

public slots:
    void visualizerClicked(QListWidgetItem* item)
    {
        emit visualizerSelected(item->text());
    }

signals:
    void visualizerSelected(QString);
};

class VisualizerWindow : public QMainWindow
{
    Q_OBJECT
private:
    ToolpathScene m_ts;

    VisualizerList m_vl;

public:
    VisualizerWindow(QWidget *parent = 0) :
        QMainWindow(parent)
    {
        QWidget *central = new QWidget(this);
        QGridLayout *layout = new QGridLayout();
        central->setLayout(layout);
        setCentralWidget(central);

        QGraphicsView *view = new GraphicsView(&m_ts, this);
        layout->addWidget(view, 1, 1, 2, 1);

        layout->addWidget(&m_vl, 1, 2, 1, 1);

        QPushButton *openFileButton = new QPushButton("Open File...", this);
        layout->addWidget(openFileButton, 2, 2, 1, 1);

        layout->setColumnStretch(1, 50);

        // let the user choose a visualizer
        connect(&m_vl, SIGNAL(visualizerSelected(QString)),
                &m_ts, SLOT(setVisualizer(QString)));

        connect(openFileButton, SIGNAL(clicked()),
                this, SLOT(open()));
    }

public slots:
    void open() {

        // create the file filter for the QFileDialog from the names of the parsers
        QString filter;
        foreach(QString f, m_ts.parsers())
        {
            filter.append(f);
            filter.append(";;");
        }

        QFileDialog fileDialog(this, QString("Open Toolpath"), QString(""), filter);
        fileDialog.setFileMode(QFileDialog::ExistingFile);
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

        // let the user choose a file
        if(fileDialog.exec() == QFileDialog::Accepted)
        {
            QFileInfo selectedFile(fileDialog.selectedFiles().at(0));
            QString selectedFilter(fileDialog.selectedFilter());

            m_ts.setParser(selectedFilter);
            m_ts.setFile(selectedFile);
        }
    }
};

#endif // EXAMPLE_H
