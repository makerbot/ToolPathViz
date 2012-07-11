#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "view/toolpathscene.h"
#include "load/visualizers.h"
#include "load/controlledvisuals.h"

/*!
  This is an example of how one might wrap the ToolpathScene in a QGraphicsView.

  ToolpathScene requires that an opengl painter is used, so we create an opengl
  widget for it to use.
  */
class ToolpathView : public QGraphicsView
{
    Q_OBJECT
private:
    ToolpathScene *m_scene;

    QGraphicsProxyWidget *m_overlay;

public:
    ToolpathView(ToolpathScene *scene, QWidget *parent = 0) :
        QGraphicsView(scene, parent),
        m_scene(scene),
        m_overlay(new QGraphicsProxyWidget())
    {
        QGLFormat glFormat(QGL::SampleBuffers | QGL::AlphaChannel);
        glFormat.setSwapInterval(1); // vsync

        QGLWidget *glWidget = new QGLWidget(glFormat, this);

        setViewport(glWidget);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        m_scene->addItem(m_overlay);
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        QGraphicsView::resizeEvent(event);

        if(scene()) {
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        }
        if(m_overlay->widget()) {
            m_overlay->resize(event->size());
        }
    }

public slots:
    void setOverlay(Visualizer* vis)
    {
        if(m_overlay->widget()) {
            // unembed the widget
            m_overlay->setWidget(0);
        }

        QWidget *overlay = vis->overlay();
        if(overlay != 0)
        {
            m_overlay->setWidget(overlay);
            m_overlay->resize(width(), height());
        }
    }
};

/*!
  A widget to list the available visualizers.
  */
class VisualizerList : public QListWidget
{
    Q_OBJECT
private:
    QMap<QString, Visualizer*> m_visMap;

public:
    VisualizerList(QWidget *parent = 0) :
        QListWidget(parent)
    {
        // v Add visualizers to this map v
        m_visMap.insert("Example Visualizer", new ExampleVisualizer());
        m_visMap.insert("Dualstrusion Visualizer", new DualstrusionVisualizer());
        m_visMap.insert("Layer Visualizer", new LayerVisualizer());
        // ^ Add visualizers to this map ^

        foreach(QString s, m_visMap.keys())
        {
            addItem(s);
        }

        connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(visualizerClicked(QListWidgetItem*)));
    }

public slots:
    void visualizerClicked(QListWidgetItem* item)
    {
        // emit the selected visualizer, or 0 if not found in map
        emit visualizerSelected(m_visMap.value(item->text(), 0));
    }

signals:
    void visualizerSelected(Visualizer*);
};

/*!
  A file dialog
  */
class ToolpathOpenDialog : public QFileDialog
{
private:
    QString m_filters;

    QMap<QString, Parser*> m_parsers;
public:
    ToolpathOpenDialog(QWidget *parent = 0) :
        QFileDialog(parent, QString("Open Toolpath"))
    {
        setFileMode(QFileDialog::ExistingFile);
        setAcceptMode(QFileDialog::AcceptOpen);
    }

    void addParser(Parser* p)
    {
        // create the file filter from the names of the parsers
        QString filter(p->name() + "(*." + p->extension() + ")");

        m_parsers.insert(filter, p);

        m_filters.append(filter + ";;");
        setFilter(m_filters);
    }

    Parser* selectedParser()
    {
        // returns the selected parser or 0 if not found
        return m_parsers.value(selectedFilter(), 0);
    }
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

        QGraphicsView *view = new ToolpathView(&m_ts, this);
        layout->addWidget(view, 1, 1, 2, 1);

        layout->addWidget(&m_vl, 1, 2, 1, 1);

        QPushButton *openFileButton = new QPushButton("Open File...", this);
        layout->addWidget(openFileButton, 2, 2, 1, 1);

        layout->setColumnStretch(1, 50);

        // let the user choose a visualizer
        connect(&m_vl, SIGNAL(visualizerSelected(Visualizer*)),
                &m_ts, SLOT(setVisualizer(Visualizer*)));
        connect(&m_vl, SIGNAL(visualizerSelected(Visualizer*)),
                view,  SLOT(setOverlay(Visualizer*)));

        connect(openFileButton, SIGNAL(clicked()),
                this, SLOT(open()));
    }

public slots:
    void open()
    {
        ToolpathOpenDialog fileDialog(this);

        // v Add parsers to this list v
        fileDialog.addParser(new FullExampleGCodeParser());
        fileDialog.addParser(new ExampleGCodeParser());
        // ^ Add parsers to this list ^

        // let the user choose a file
        if(fileDialog.exec() == QFileDialog::Accepted)
        {
            QFileInfo selectedFile(fileDialog.selectedFiles().at(0));
            m_ts.setFile(selectedFile);

            m_ts.setParser(fileDialog.selectedParser());
        }
    }
};

#endif // EXAMPLE_H
