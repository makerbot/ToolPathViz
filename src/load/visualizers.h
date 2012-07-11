#ifndef VISUALIZERS_H
#define VISUALIZERS_H

#include "src/model/step.h"
#include "src/model/toolpath.h"
#include "src/model/visual.h"

/*!
  The base class for all visualizers.

  To produce a visual from a parsed toolpath subclass
  Visualizer and reimplement the methods for reading those Steps that
  are needed for producing the visual desired.

  To display your visualizer in the example (src/main.cpp),
  add your parser to the map in src/example.h
  */
class Visualizer
{
public:
    /** Changes the toolpath this visualizer displays */
    virtual void setToolpath(const Toolpath&) = 0;

    /** Renders the toolpath using openGL calls */
    virtual void renderGL() = 0;

    /** Some Visualizers may have controls.
        The default implementation returns null */
    virtual QWidget* overlay();

    /** Virtual destructor */
    virtual ~Visualizer() = 0;
};

class BasicVisualizer : public Visualizer
{
public:
    BasicVisualizer() :
        Visualizer(),
        m_visual(new GroupItem())
    {}

    void setToolpath(const Toolpath &);

    void renderGL() {
        m_visual->renderGL();
    }

    virtual ~BasicVisualizer();

protected:
    GroupItem* m_visual;

    virtual void moveAbsoluteStep(const MoveAbsoluteStep*) {}
    virtual void setFeedrateStep(const SetFeedrateStep*) {}
    virtual void setToolheadStep(const SetToolheadStep*) {}
    virtual void newLayerStep(const NewLayerStep*) {}
};

/*!
  A simple example of a visualizer. Draws white lines for all moves.
  */
class ExampleVisualizer : public BasicVisualizer
{
private:
    /** the previous point */
    QVector3D prev;
    /** is this the first point? false after the first point is read. */
    bool firstPoint;

public:
    ExampleVisualizer();

protected:
    void moveAbsoluteStep(const MoveAbsoluteStep*);
};

/*!
  A slightly more complex example of a visualizer. Draws different colors for
  different heads.
  */
class DualstrusionVisualizer : public BasicVisualizer
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
    void moveAbsoluteStep(const MoveAbsoluteStep*);
    void setToolheadStep(const SetToolheadStep*);
};

#endif // VISUALIZERS_H
