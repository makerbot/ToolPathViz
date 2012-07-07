#include "visualizers.h"

QMap<QString, Visualizer*> visualizerMap()
{
    QMap<QString, Visualizer*> visualizers;

    // v Add visualizers to this map v
    visualizers.insert("Example Visualizer", new ExampleVisualizer());
    visualizers.insert("Dualstrusion Visualizer", new DualstrusionVisualizer());
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

