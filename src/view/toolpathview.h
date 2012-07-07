#ifndef TOOLPATHVIEW_H
#define TOOLPATHVIEW_H

#include <QGraphicsView>

#include <QFileInfo>
#include <QGraphicsLayout>
#include <QGraphicsScene>

/*!
 GCodeView is the widget that displays a file as toolpaths.

 To mount transparent controls on top of this widget it's best
 to pass them into the setOverlay() function.
  */
class ToolpathView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ToolpathView(QWidget *parent = 0);
    qreal x;
signals:
    
public slots:
    /** opens a file for visualization */
    void open(QFileInfo);
    /** closes the currently open file */
    void close();

    /** Sets the passed widget as the gui overlay on this widget */
    void setOverlay(QWidget*);
    /** Sets the passed QGraphicsLayout as the gui overlay on this widget */
    void setOverlay(QGraphicsLayout*);
    
};

#endif // TOOLPATHVIEW_H
