#include "visual.h"

void Line::renderGL()
{
    glColor3i(color.red(), color.green(), color.blue());
    glBegin(GL_LINES);
    glVertex3d(from.x(), from.y(), from.z());
    glVertex3d(to.x(), to.y(), to.z());
    glEnd();
}

void Visual::add(const VisualItem *const item)
{
    items.append(item);
}

void Visual::renderGL()
{
    foreach(VisualItem* item, items)
    {
        item->renderGL();
    }
}
