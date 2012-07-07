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
    virtual void moveAbsoluteStep(const MoveAbsoluteStep&) {}
};

#endif // VISUALIZERS_H
