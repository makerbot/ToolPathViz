#include "gcodeviewapplication.h"
#include "mainwindow.h"

GCodeViewApplication::GCodeViewApplication(int argc, char *argv[]) :
    QApplication(argc, argv)
{
    setOrganizationName("MakerBot Industries");
    setApplicationName("Toolpath Helper");
}


void GCodeViewApplication::LoadFile(QString fileName) {
    // TODO: Check if this file has been loaded before attempting to load it again.
    MainWindow * myNewWindow = new MainWindow();
    myNewWindow->show();
    myNewWindow->loadFile(fileName);

    // Update the window menus across the app.
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateWindowMenu();
    }
}
