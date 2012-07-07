#include "toolpathview.h"

#include "src/load/parsers.h"
#include "src/load/visualizers.h"

ToolpathView::ToolpathView(QWidget *parent) :
    QGraphicsView(parent),
    m_scene(this)
{
    QGLFormat glFormat(QGL::SampleBuffers | QGL::AlphaChannel);
    glFormat.setSwapInterval(1); // vsync

    QGLWidget *glWidget = new QGLWidget(glFormat, this);

    setScene(&m_scene);

    setViewport(glWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void ToolpathView::parse()
{
    m_toolpath.clear();

    ParserFactory pFact;

    pFact.loadToolpath(m_file, m_parser, m_toolpath);
}

void ToolpathView::visualize()
{
    Visualizer* visualizer = visualizerMap().value(m_visualizer, 0);
    if(visualizer != 0)
        m_visual = visualizer->visualize(m_toolpath);
}

QStringList ToolpathView::parsers()
{
    return ParserFactory().parsers();
}

QStringList ToolpathView::visualizers()
{
    return visualizerList();
}

void ToolpathView::setFile(QFileInfo file)
{
    m_file = file;

    parse();

    visualize();
}

void ToolpathView::setParser(QString parser)
{
    if(m_parser == parser)
        return;

    m_parser = parser;

    parse();

    visualize();
}

void ToolpathView::setVisualizer(QString visualizer)
{
    if(m_visualizer == visualizer)
        return;

    m_visualizer = visualizer;

    visualize();
}

void ToolpathView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if(scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
}

void ToolpathView::setOverlay(QGraphicsLayout *)
{

}

void ToolpathView::setOverlay(QWidget *)
{

}
