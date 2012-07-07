#ifndef STEP_H
#define STEP_H

#include <QtGlobal>

#include <QtOpenGL>

/*!
    The base class for all actions a toolpath file might contain.
  */
class Step
{
public:
    /** The source of this Step in the original file.
        Usually used to describe the line number it came from,
        could also refer to a byte or a packet. */
    uint origin;

    Step(uint o) : origin(o) {}

    virtual void enablePolymorphism() {}
};

/*!
  MoveAbsoluteStep refers to a movement to a position in absolute coordinates
  */
class MoveAbsoluteStep : public Step
{
public:
    qreal x, y, z, a, b;

    MoveAbsoluteStep(uint o, qreal x, qreal y, qreal z, qreal a, qreal b) :
        Step(o), x(x), y(y), z(z), a(a), b(b) {}
};

#endif // STEP_H
