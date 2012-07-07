#include "visualizers.h"

QMap<QString, Visualizer*> visualizerMap()
{
    QMap<QString, Visualizer*> visualizers;

    // v Add visualizers to this map v
    visualizers.insert("Example Visualizer", new ExampleVisualizer());
    // ^ Add visualizers to this map ^

    return visualizers;
}

QStringList visualizerList()
{
    QStringList visualizers;

    foreach(QString s, visualizerMap().keys())
        visualizers << s;

    return visualizers;
}

Visual Visualizer::visualize(const Toolpath &path)
{
    Visual result;
    // v Add static_cast check to this loop v
    foreach(const Step *s, path)
    {
        const MoveAbsoluteStep *mas =
                dynamic_cast<const MoveAbsoluteStep *>(s);
        if(mas != 0)
        {
            moveAbsoluteStep(mas, result);
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
    visual.add(new Line(*step, QColor(255, 255, 255), prev, newPoint));
    prev = newPoint;
}
