#ifndef VISUAL_H
#define VISUAL_H

#include <QColor>

#include <step.h>

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
};

class Visual
{
public:
    Visual();

    /** uses the current openGL context to draw itself */
    renderGL();
};

#endif // VISUAL_H
