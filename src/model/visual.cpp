#include "visual.h"

void GroupItem::add(VisualItem *item)
{
    m_items.append(item);
}

void GroupItem::renderGL() const
{
    foreach(VisualItem *item, m_items)
    {
        item->renderGL();
    }
}

void Line::renderGL() const
{
    glLineWidth(1);
    glColor4f(m_color.redF(), m_color.greenF(), m_color.blueF(), m_color.alphaF());
    glBegin(GL_LINES);
    glVertex3d(m_from.x(), m_from.y(), m_from.z());
    glVertex3d(m_to.x(), m_to.y(), m_to.z());
    glEnd();
}
