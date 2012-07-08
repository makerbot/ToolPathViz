#ifndef VISUAL_H
#define VISUAL_H

#include <QColor>
#include <QList>

#include "step.h"

/*!
  The base class for displayable shapes.

  If you need to display a shape that is not yet supported, just subclass
  this and implement renderGL().
  */
struct VisualItem
{
    /** The Step that caused this VisualItem to be created. */
    const Step& origin;

    /** The color with which this item should be displayed */
    const QColor color;

    VisualItem(const Step& o, const QColor c) :
        origin(o),
        color(c)
    {}

    /** Renders this item. Should leave the view matrices untouched. */
    virtual void renderGL() const =0;
};

/*!
  A simple line.
  */
struct Line : public VisualItem
{
    const QVector3D from, to;

    Line(const Step& o, const QColor c, const QVector3D f, const QVector3D t) :
        VisualItem(o, c),
        from(f),
        to(t)
    {}

    void renderGL() const;
};

/*!
    A collection of VisualItems (shapes, lines, etc.) that renders
    itself.

    If you need to display a shape that is not yet supported, just subclass
    VisualItem and implement its renderGL() function.
  */
class Visual
{
private:
    QList<const VisualItem*> items;

public:
    void add(const VisualItem*);

    /** uses the current openGL context to draw itself */
    void renderGL();
};

#endif // VISUAL_H
