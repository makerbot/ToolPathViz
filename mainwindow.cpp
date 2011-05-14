#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gcodeview.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->graphicsView->loadModel("/Users/mattmets/Projects/repg-workspace/ReplicatorG/examples/wfu_cbi_skull.gcode");
    ui->graphicsView->loadModel("/Users/mattmets/Projects/repg-workspace/ReplicatorG/examples/Mini_Snake.gcode");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_GCode_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "/home",
                                                tr("GCode (*.gcode)"));

    ui->graphicsView->loadModel(file);
}
