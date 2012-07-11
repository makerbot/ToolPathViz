#ifndef TOOLPATHSCENE_H
#define TOOLPATHSCENE_H

#include <QGraphicsScene>

#include "src/load/parsers.h"
#include "src/load/visualizers.h"
#include "src/model/visual.h"
#include "src/model/toolpath.h"
#include "viewmodel.h"
#include "viewpointcontroller.h"

/*!
  ToolpathScene is a QGraphicsScene for displaying Toolpaths.

  Set a file, a parser, and a visualizer to display the toolpath.
  Valid parser and visualizer names are listed by parsers() and visualizers().

  Note: ToolpathScene requires an opengl painter on QGraphicsViews that view it.
  */
class ToolpathScene : public QGraphicsScene
{
    Q_OBJECT
private:

    /** The file to load from */
    QFileInfo m_file;

    /** The parser with which to load the file */
    Parser *m_parser;

    /** The model of the toolpath produced by the parser */
    Toolpath m_toolpath;

    /** The visualizer with which to create a visual from a toolpath */
    Visualizer *m_visualizer;

    /** Have the parser re-parse the file */
    void parse();

    /** Have the visualizer re-analyze the toolpath */
    void visualize();

    /** for setting up opengl stuff */
    ViewModel m_viewModel;

    /** Takes user input and uses it to control the view model */
    ViewpointController *m_vpController;

public:
    explicit ToolpathScene(QObject *parent = 0);
    
    /** reimplemented to render the visual as the background of the widget */
    void drawBackground(QPainter *painter, const QRectF &rect);

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
    void wheelEvent( QGraphicsSceneWheelEvent *event );

public slots:
    /** opens a file for visualization */
    void setFile(QFileInfo);
    /** Sets the parser to use when reading files */
    void setParser(Parser*);
    /** Sets the visualizer to use for creating visuals from files */
    void setVisualizer(Visualizer*);
};

#endif // TOOLPATHSCENE_H
