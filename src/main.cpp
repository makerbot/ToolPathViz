#include <QtGui/QApplication>
#include <QMainWindow>

#include "view/toolpathview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mw;
    mw.setCentralWidget(new ToolpathView(&mw));

    return a.exec();
}
