#include <QtGui/QApplication>
#include <QMainWindow>

#include "load/parsers.h"
#include "load/visualizers.h"
#include "view/toolpathscene.h"

#include "example.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mw;
    ToolpathScene ts;
    mw.setCentralWidget(new GraphicsView(&ts, &mw));
    mw.show();

    // create the file filter for the QFileDialog from the names of the parsers
    QString filter;
    foreach(QString f, ts.parsers())
    {
        filter.append(f);
        filter.append(";;");
    }

    QFileDialog fileDialog(&mw, QString("Open Toolpath"), QString(""), filter);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    // let the user choose a file
    if(fileDialog.exec() == QFileDialog::Accepted)
    {
        QFileInfo selectedFile(fileDialog.selectedFiles().at(0));
        QString selectedFilter(fileDialog.selectedFilter());

        ts.setParser(selectedFilter);
        ts.setFile(selectedFile);
    }

    // let the user choose a visualizer
    VisualizerDialog visualizerDialog(&mw);
    QObject::connect(&visualizerDialog, SIGNAL(visualizerSelected(QString)),
                     &ts, SLOT(setVisualizer(QString)));

    visualizerDialog.show();
    visualizerDialog.raise();
    visualizerDialog.activateWindow();

    return a.exec();
}
