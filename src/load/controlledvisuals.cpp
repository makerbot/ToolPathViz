#include "controlledvisuals.h"

#include <limits>

IndexedVisual::IndexedVisual() :
    QObject(0),
    m_items(),
    m_high(INT_MIN),
    m_low(INT_MAX),
    m_min(INT_MAX),
    m_max(INT_MIN),
    m_overlay(new QFrame(0)),
    m_highSlider(new QSlider(Qt::Vertical)),
    m_lowSlider(new QSlider(Qt::Vertical))
{
    QGridLayout *layout = new QGridLayout(0);

    layout->addWidget(m_highSlider, 1, 2, 1, 1, Qt::AlignRight);
    layout->addWidget(m_lowSlider, 2, 2, 1, 1, Qt::AlignRight);
    layout->setColumnStretch(1, 100);

    m_overlay->setWindowOpacity(.999);
    m_overlay->setAttribute(Qt::WA_NoSystemBackground);
    m_overlay->setLayout(layout);

    connect(m_highSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setRangeHigh(int)));
    connect(m_lowSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setRangeLow(int)));
}

IndexedVisual::~IndexedVisual()
{
    if(m_overlay->parent() == 0)
        delete m_overlay;
}

void IndexedVisual::setRangeLow(int low)
{
    m_low = qMax(low, m_min);

    // always show something
    if(m_low >= m_high) {
        m_high = m_low;
        m_highSlider->setValue(m_high);
    }
}
void IndexedVisual::setRangeHigh(int high)
{
    m_high = qMin(high, m_max);

    // always show something
    if(m_high <= m_low) {
        m_low = m_high;
        m_lowSlider->setValue(m_low);
    }
}

int IndexedVisual::rangeLow()
{
    return m_low;
}
int IndexedVisual::rangeHigh()
{
    return m_high;
}

int IndexedVisual::rangeMin()
{
    return m_min;
}
int IndexedVisual::rangeMax()
{
    return m_max;
}

void IndexedVisual::add(VisualItem *item, int layer)
{
    if(layer > m_max) {
        m_max = layer;
        m_highSlider->setMaximum(m_max);
        m_lowSlider->setMaximum(m_max);
    }
    if(layer < m_min) {
        m_min = layer;
        m_highSlider->setMinimum(m_min);
        m_lowSlider->setMinimum(m_min);
    }

    m_items.insertMulti(layer, item);
}

void IndexedVisual::renderGL() const
{
    for(int i = m_low; i <= m_high; i++)
    {
        foreach(VisualItem* item, m_items.values(i))
        {
            item->renderGL();
        }
    }
}

QWidget *IndexedVisual::overlay()
{
    return m_overlay;
}

LayerVisualizer::LayerVisualizer() :
    BasicVisualizer(),
    m_prev(),
    m_firstPoint(true),
    m_layer(0),
    m_index()
{
    m_visual->add(&m_index);
}

QWidget* LayerVisualizer::overlay()
{
    return m_index.overlay();
}

void LayerVisualizer::moveAbsoluteStep(const MoveAbsoluteStep *step)
{
    QVector3D newPoint(step->x, step->y, step->z);
    if(not m_firstPoint)
        m_index.add(new Line(QColor(255, 255, 255), m_prev, newPoint), m_layer);
    else
        m_firstPoint = false;
    m_prev = newPoint;
}

void LayerVisualizer::newLayerStep(const NewLayerStep *step)
{
    m_layer++;
}
