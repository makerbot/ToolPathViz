#include "toolpathscene.h"

#include "src/load/parsers.h"
#include "src/load/visualizers.h"

ToolpathScene::ToolpathScene(QObject *parent) :
    QGraphicsScene(parent),
    m_file(),
    m_parser(0),
    m_toolpath(),
    m_visualizer(0),
    m_viewModel(),
    m_vpController(new AzimuthZenithController(m_viewModel, this))
{
    connect(&m_viewModel, SIGNAL(updated()), this, SLOT(update()));
    connect(this, SIGNAL(sceneRectChanged(QRectF)),
            &m_viewModel, SLOT(resize(QRectF)));
}

void ToolpathScene::parse()
{
    m_toolpath.clear();

    if(m_parser)
        m_parser->loadToolpath(m_file, m_toolpath);
}

void ToolpathScene::visualize()
{
    if(m_visualizer)
        m_visualizer->setToolpath(m_toolpath);

    update();
}

void ToolpathScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    int engineType = painter->paintEngine()->type();
    if (engineType != QPaintEngine::OpenGL
            && engineType != QPaintEngine::OpenGL2) {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget \
                 to be set as viewport on the graphics view");
        return;
    }

    m_viewModel.setupView();
    if(m_visualizer)
        m_visualizer->renderGL();
}

void ToolpathScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(not event->isAccepted())
        m_vpController->mousePressEvent(event);
}

void ToolpathScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if(not event->isAccepted())
        m_vpController->mouseMoveEvent(event);
    update();
}

void ToolpathScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if(not event->isAccepted())
        m_vpController->mouseReleaseEvent(event);
}

void ToolpathScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if(not event->isAccepted())
        m_vpController->wheelEvent(event);
}

void ToolpathScene::setFile(QFileInfo file)
{
    m_file = file;

    parse();

    visualize();
}

void ToolpathScene::setParser(Parser *parser)
{
    if(m_parser == parser)
        return;

    m_parser = parser;

    parse();

    visualize();
}

void ToolpathScene::setVisualizer(Visualizer *visualizer)
{
    if(m_visualizer == visualizer)
        return;

    m_visualizer = visualizer;

    visualize();
}
