#include <QtGui/QApplication>
#include <QMainWindow>

#include "view/toolpathview.h"
#include "load/parsers.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mw;
    ToolpathView tv(&mw);
    mw.setCentralWidget(&tv);
    mw.show();

    // create the file filter for the QFileDialog from the names of the parsers
    QString filter;
    foreach(QString f, tv.parsers())
    {
        filter.append(f);
        filter.append(";;");
    }

    QFileDialog fileDialog(&tv, QString("Open Toolpath"), QString(""), filter);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    // let the user choose a file
    if(fileDialog.exec() == QFileDialog::Accepted)
    {
        QFileInfo selectedFile(fileDialog.selectedFiles().at(0));
        QString selectedFilter(fileDialog.selectedFilter());

        tv.setParser(selectedFilter);
        tv.setFile(selectedFile);
    }

    // let the user choose a visualizer
    //???

    return a.exec();
}
