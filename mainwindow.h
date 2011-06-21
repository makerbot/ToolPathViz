#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Load a file with the given name
    void loadFile(const QString &fileName);

    // TODO: delme?
    bool hasFile();

    void updateWindowMenu();
private slots:
    void on_actionLoad_GCode_triggered();

    void on_LayerHeight_sliderMoved(int position);

    void openRecentFile();
    void on_actionClose_triggered();

    void on_actionExport_Gcode_File_triggered();

private:
    Ui::MainWindow *ui;

    QString curFile;

    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];
};

#endif // MAINWINDOW_H
