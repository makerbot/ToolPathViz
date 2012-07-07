#ifndef TOOLPATHVIEW_H
#define TOOLPATHVIEW_H

#include <QGraphicsView>

#include <QFileInfo>
#include <QGLWidget>
#include <QGraphicsLayout>

#include "toolpathscene.h"
#include "src/model/toolpath.h"
#include "src/model/visual.h"

/*!
 GCodeView is the widget that displays a file as toolpaths.

 Set a file, a parser, and a visualizer to display the toolpath.
 Valid parser and visualizer names are listed by parsers() and visualizers().

 To mount transparent controls on top of this widget it's best
 to pass them into the setOverlay() function.
  */
class ToolpathView : public QGraphicsView
{
    Q_OBJECT
private:

    /** The file to load from */
    QFileInfo m_file;

    /** The parser with which to load the file */
    QString m_parser;

    /** The model of the toolpath produced by the parser */
    Toolpath const m_toolpath;

    /** The visualizer with which to create a visual from a toolpath */
    QString m_visualizer;

    /** The visual produced, to be displayed by the ToolpathScene */
    Visual const m_visual;

    /** The QGraphicsScene responsible for rendering visuals */
    ToolpathScene m_scene;

    /** Have the parser re-parse the file */
    void parse();

    /** Have the visualizer re-analyze the toolpath */
    void visualize();

public:
    explicit ToolpathView(QWidget *parent = 0);

    /** The list of valid parser names */
    QStringList parsers();
    /** The list of valid visualizer names */
    QStringList visualizers();

protected:
    /** reimplemented to resize our QGraphicsScene */
    void resizeEvent(QResizeEvent *event);

signals:
    
public slots:

    /** opens a file for visualization */
    void setFile(QFileInfo);
    /** Sets the parser to use when reading files */
    void setParser(QString);
    /** Sets the visualizer to use for creating visuals from files */
    void setVisualizer(QString);

    /** Sets the passed widget as the gui overlay on this widget */
    void setOverlay(QWidget*);
    /** Sets the passed QGraphicsLayout as the gui overlay on this widget */
    void setOverlay(QGraphicsLayout*);

};

#endif // TOOLPATHVIEW_H
