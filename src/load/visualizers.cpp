#include "visualizers.h"

Visual Visualizer::visualize(const Toolpath &path)
{
    // \/ Add dynamic_cast check to this loop \/
    foreach(Step const *const s, path)
    {
        MoveAbsoluteStep const *const mas =
                dynamic_cast<MoveAbsoluteStep const *const>(s);
        if(mas != 0)
        {
            moveAbsoluteStep(mas);
            continue;
        }
    }
    // /\ Add dynamic_cast check to this loop /\
}

ExampleVisualizer::ExampleVisualizer() :
    firstPoint(true)
{}

void ExampleVisualizer::moveAbsoluteStep(MoveAbsoluteStep const *const step,
                                         Visual& visual)
{
    QVector3D newPoint(step->x, step->y, step->z);
    visual.add(new Line(step, QColor(255, 255, 255), prev, newPoint));
    prev = newPoint;
}
