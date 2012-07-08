#include <QtGui/QApplication>
#include <QMainWindow>

#include "load/parsers.h"
#include "load/visualizers.h"
#include "view/toolpathscene.h"

#include "example.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VisualizerWindow vw;
    vw.resize(700, 400);
    vw.show();

    return a.exec();
}
