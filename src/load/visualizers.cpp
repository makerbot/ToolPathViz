#include "visualizers.h"

Visual Visualizer::visualize(const Toolpath &path)
{
    Visual result;
    // v Add dynamic_cast check to this loop v
    foreach(const Step *s, path)
    {
        const MoveAbsoluteStep *mas =
                dynamic_cast<const MoveAbsoluteStep *>(s);
        if(mas != 0)
        {
            moveAbsoluteStep(mas, result);
            continue;
        }

        const SetFeedrateStep *sfs =
                dynamic_cast<const SetFeedrateStep *>(s);
        if(sfs != 0)
        {
            setFeedrateStep(sfs, result);
            continue;
        }

        const SetToolheadStep *sts =
                dynamic_cast<const SetToolheadStep *>(s);
        if(sts != 0)
        {
            setToolheadStep(sts, result);
            continue;
        }

        const NewLayerStep *nls =
                dynamic_cast<const NewLayerStep *>(s);
        if(nls != 0)
        {
            newLayerStep(nls, result);
            continue;
        }
    }
    // ^ Add dynamic_cast check to this loop ^

    return result;
}

ExampleVisualizer::ExampleVisualizer() :
    firstPoint(true)
{}

void ExampleVisualizer::moveAbsoluteStep(const MoveAbsoluteStep *step,
                                         Visual& visual)
{
    QVector3D newPoint(step->x, step->y, step->z);
    if(not firstPoint)
        visual.add(new Line(*step, QColor(255, 255, 255), prev, newPoint));
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

void DualstrusionVisualizer::moveAbsoluteStep(const MoveAbsoluteStep *step,
                                             Visual& visual)
{
    QVector3D newPoint(step->x, step->y, step->z);
    QColor lineColor;
    if(toolhead == 0)
        lineColor = rightToolheadColor;
    else if(toolhead == 1)
        lineColor = leftToolheadColor;

    if(not firstPoint)
        visual.add(new Line(*step, lineColor, prev, newPoint));
    else
        firstPoint = false;
    prev = newPoint;
}

void DualstrusionVisualizer::setToolheadStep(const SetToolheadStep *step,
                                             Visual& visual)
{
    toolhead = step->toolhead;
}
