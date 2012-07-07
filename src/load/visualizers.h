#ifndef VISUALIZERS_H
#define VISUALIZERS_H

#include "src/model/step.h"
#include "src/model/toolpath.h"
#include "src/model/visual.h"

/*!
    The base class for all visualizers.

    To produce a visual from a parsed toolpath one need only subclass
    Visualizer and reimplement the methods for reading those Steps that
    are needed for producing the visual desired, then add your visualizer to the
    Map in visualizers.cpp.
  */
class Visualizer
{
public:
    /** called to create a displayable Visual */
    Visual visualize(const Toolpath&);

protected:
    virtual void moveAbsoluteStep(const MoveAbsoluteStep*, Visual&) {}
    virtual void setFeedrateStep(const SetFeedrateStep*, Visual&) {}
    virtual void setToolheadStep(const SetToolheadStep*, Visual&) {}
    virtual void newLayerStep(const NewLayerStep*, Visual&) {}
};

class ExampleVisualizer : public Visualizer
{
private:
    /** the previous point */
    QVector3D prev;
    /** is this the first point? false after the first point is read. */
    bool firstPoint;

public:
    ExampleVisualizer();

protected:
    void moveAbsoluteStep(const MoveAbsoluteStep*, Visual&);
};

class DualstrusionVisualizer : public Visualizer
{
private:
    /** the previous point */
    QVector3D prev;
    /** is this the first point? false after the first point is read. */
    bool firstPoint;
    /** the current toolhead */
    int toolhead;

public:
    DualstrusionVisualizer();

protected:
    void moveAbsoluteStep(const MoveAbsoluteStep*, Visual&);
    void setToolheadStep(const SetToolheadStep*, Visual&);
};

QStringList visualizerList();
QMap<QString, Visualizer*> visualizerMap();

#endif // VISUALIZERS_H
