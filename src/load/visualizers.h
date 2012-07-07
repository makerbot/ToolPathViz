#ifndef VISUALIZERS_H
#define VISUALIZERS_H

#include "src/model/step.h"
#include "src/model/toolpath.h"
#include "src/model/visual.h"

/*!
    The base class for all visualizers.

    To produce a visual from a parsed toolpath one need only subclass
    Visualizer and reimplement the methods for reading those Steps that
    are needed for producing the visual desired.
  */
class Visualizer
{
public:
    /** called to create a dispalyable Visual */
    Visual visualize(const Toolpath&);

protected:
    virtual void moveAbsoluteStep(MoveAbsoluteStep const *const,
                                  Visual&)
    {}
};

class ExampleVisualizer : public Visualizer
{
private:
    /** the previous point */
    QVector3D prev;
    /** is this the first point? false after the first point is read. */
    bool firstPoint;

    ExampleVisualizer();
protected:
    void moveAbsoluteStep(MoveAbsoluteStep const *const, Visual&);
};

#endif // VISUALIZERS_H
