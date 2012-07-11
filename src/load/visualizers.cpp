#include "visualizers.h"

#include <QVector3D>

QWidget* Visualizer::overlay()
{
    return 0;
}

Visualizer::~Visualizer()
{

}

void BasicVisualizer::setToolpath(const Toolpath &path)
{
    // v Add dynamic_cast check to this loop v
    foreach(const Step *s, path)
    {
        const MoveAbsoluteStep *mas =
                dynamic_cast<const MoveAbsoluteStep *>(s);
        if(mas != 0)
        {
            moveAbsoluteStep(mas);
            continue;
        }

        const SetFeedrateStep *sfs =
                dynamic_cast<const SetFeedrateStep *>(s);
        if(sfs != 0)
        {
            setFeedrateStep(sfs);
            continue;
        }

        const SetToolheadStep *sts =
                dynamic_cast<const SetToolheadStep *>(s);
        if(sts != 0)
        {
            setToolheadStep(sts);
            continue;
        }

        const NewLayerStep *nls =
                dynamic_cast<const NewLayerStep *>(s);
        if(nls != 0)
        {
            newLayerStep(nls);
            continue;
        }
    }
    // ^ Add dynamic_cast check to this loop ^
}

BasicVisualizer::~BasicVisualizer()
{
    delete m_visual;
}

ExampleVisualizer::ExampleVisualizer() :
    firstPoint(true)
{}

void ExampleVisualizer::moveAbsoluteStep(const MoveAbsoluteStep *step)
{
    QVector3D newPoint(step->x, step->y, step->z);
    if(not firstPoint)
        m_visual->add(new Line(QColor(255, 255, 255), prev, newPoint));
    else
        firstPoint = false;
    prev = newPoint;
}

// for left & right toolheads.
const QColor leftToolheadColor(192, 0, 0);
const QColor rightToolheadColor(0, 0, 192);

DualstrusionVisualizer::DualstrusionVisualizer() :
    firstPoint(true),
    toolhead(0)
{}

void DualstrusionVisualizer::moveAbsoluteStep(const MoveAbsoluteStep *step)
{
    QVector3D newPoint(step->x, step->y, step->z);
    QColor lineColor;
    if(toolhead == 0)
        lineColor = rightToolheadColor;
    else if(toolhead == 1)
        lineColor = leftToolheadColor;

    if(not firstPoint)
        m_visual->add(new Line(lineColor, prev, newPoint));
    else
        firstPoint = false;
    prev = newPoint;
}

void DualstrusionVisualizer::setToolheadStep(const SetToolheadStep *step)
{
    toolhead = step->toolhead;
}
