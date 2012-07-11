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
class VisualItem
{
public:
    /** Renders this item. Should leave the view matrices untouched. */
    virtual void renderGL() const =0;
};

/*!

  */
class GroupItem : public VisualItem
{
private:
    QList<VisualItem*> m_items;

public:
    GroupItem() : VisualItem() {}

    void add(VisualItem*);

    void renderGL() const;
};

/*!
  A simple line.
  */
class Line : public VisualItem
{
private:
    const QVector3D m_from, m_to;

    /** The color with which this item should be displayed */
    const QColor m_color;

public:
    Line(const QColor c, const QVector3D f, const QVector3D t) :
        VisualItem(),
        m_from(f),
        m_to(t),
        m_color(c)
    {}

    void renderGL() const;
};



#endif // VISUAL_H
