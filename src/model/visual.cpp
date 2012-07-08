#include "visual.h"

void Line::renderGL() const
{
    glLineWidth(1);
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    glBegin(GL_LINES);
    glVertex3d(from.x(), from.y(), from.z());
    glVertex3d(to.x(), to.y(), to.z());
    glEnd();
}

void Visual::add(const VisualItem *item)
{
    items.append(item);
}

void Visual::renderGL()
{
    foreach(const VisualItem *item, items)
    {
        item->renderGL();
    }
}
