#ifndef STEP_H
#define STEP_H

#include <QtGlobal>

#include <QtOpenGL>

/*!
    The base class for all actions a toolpath file might contain.

    To add a new kind of step, subclass Step, add a function to Visualizer, and
    add a case to the loop in visualizers.cpp
  */
struct Step
{
    /** The source of this Step in the original file.
        Usually used to describe the line number it came from,
        could also refer to a byte or a packet. */
    uint origin;

    Step(uint o) : origin(o) {}

    // this is a hack to allow dynamic_cast<>()ing
    virtual void enablePolymorphism() {}
};

/*!
  Refers to a movement to a position in absolute coordinates
  */
struct MoveAbsoluteStep : public Step
{
    qreal x, y, z, a, b;

    MoveAbsoluteStep(uint o, qreal x, qreal y, qreal z, qreal a, qreal b) :
        Step(o), x(x), y(y), z(z), a(a), b(b) {}
};

/*!
  Describes a change in feedrate
  */
struct SetFeedrateStep : public Step
{
    qreal feedrate;

    SetFeedrateStep(uint o, qreal f) :
        Step(o), feedrate(f) {}
};

/*!
  Designates a request for a specific toolhead
  */
struct SetToolheadStep : public Step
{
    int toolhead;

    SetToolheadStep(uint o, int t) :
        Step(o), toolhead(t) {}
};

/*!
  Indicates a new layer of extrusion has started
  */
struct NewLayerStep : public Step
{
    qreal zHeight;

    NewLayerStep(uint o, qreal z) :
        Step(o), zHeight(z) {}
};

#endif // STEP_H
