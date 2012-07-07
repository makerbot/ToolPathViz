#include "toolpathscene.h"

#include "src/load/parsers.h"
#include "src/load/visualizers.h"

ToolpathScene::ToolpathScene( QObject *parent) :
    QGraphicsScene(parent)
{
}

void ToolpathScene::parse()
{
    m_toolpath.clear();

    ParserFactory pFact;

    pFact.loadToolpath(m_file, m_parser, m_toolpath);
}

void ToolpathScene::visualize()
{
    Visualizer* visualizer = visualizerMap().value(m_visualizer, 0);
    if(visualizer != 0)
        m_visual = visualizer->visualize(m_toolpath);
}

QStringList ToolpathScene::parsers()
{
    return ParserFactory().parsers();
}

QStringList ToolpathScene::visualizers()
{
    return visualizerList();
}

void ToolpathScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    int engineType = painter->paintEngine()->type();
    if (engineType != QPaintEngine::OpenGL
            && engineType != QPaintEngine::OpenGL2) {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
    }

    m_visual.renderGL();
}

void ToolpathScene::setFile(QFileInfo file)
{
    m_file = file;

    parse();

    visualize();
}

void ToolpathScene::setParser(QString parser)
{
    if(m_parser == parser)
        return;

    m_parser = parser;

    parse();

    visualize();
}

void ToolpathScene::setVisualizer(QString visualizer)
{
    if(m_visualizer == visualizer)
        return;

    m_visualizer = visualizer;

    visualize();
}
