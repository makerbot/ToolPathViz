#ifndef CONTROLLEDVISUALS_H
#define CONTROLLEDVISUALS_H

#include "visualizers.h"

#include <QFrame>
#include <QSlider>

/*!
  Holds groups of items at different indices,
  basically just items mapped to numbers.
  Allows a limited range of those indexes to be shown.
  */
struct IndexedVisual : public QObject, public VisualItem
{
    Q_OBJECT
private:
    /** used as a multimap */
    QMap<int, VisualItem*> m_items;

    /** keep track of range to display */
    int m_high, m_low, m_min, m_max;

    /** our overlay, returned by overlay() */
    QFrame *m_overlay;

    /** our two sliders for controlling the high
        and low end of the range to be displayed */
    QSlider *m_highSlider, *m_lowSlider;

public:

    IndexedVisual();
    ~IndexedVisual();

    void add(VisualItem*, int layer);

    int rangeLow();
    int rangeHigh();

    int rangeMin();
    int rangeMax();

    void renderGL() const;

    QWidget* overlay();

public slots:

    void setRangeLow(int low);
    void setRangeHigh(int high);
};

/*!
  One use for an IndexedVisual is displaying the different layers of a toolpath.
  LayerVisualizer delegates to IndexedVisual, adding lines indexed by layer as
  defined by the newLayerStep.
  */
class LayerVisualizer : public BasicVisualizer
{
private:
    /** the previous point */
    QVector3D m_prev;
    /** is this the first point? false after the first point is read. */
    bool m_firstPoint;

    /**  */
    int m_layer;

    /**  */
    IndexedVisual m_index;

public:

    LayerVisualizer();

    QWidget *overlay();

    void moveAbsoluteStep(const MoveAbsoluteStep*);
    void newLayerStep(const NewLayerStep*);
};

#endif // CONTROLLEDVISUALS_H
