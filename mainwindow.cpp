#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <iostream>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gcodeview.h"
#include "gcodeviewapplication.h"
//#include "slicingdialog.h"

#include "mgl/miracle.h"
#include "mgl/configuration.h"

using namespace mgl;
using namespace std;


class Progress : public ProgressBar
{
    QLabel &taskLabel; QProgressBar& progress;
public:
    Progress(QLabel *taskLabelp, QProgressBar* progressp)
        :taskLabel(*taskLabelp), progress(*progressp)
    {

    }

    void onTick(const char* taskName, unsigned int count, unsigned int tick)
    {
        if(tick==0)
        {
            taskLabel.setText(taskName);
            progress.setMinimum(0);
            progress.setMaximum(count);

        }
        progress.setValue(tick+1);
        qApp->processEvents();
        // cout << taskName << " tick: " << tick << "/" << count << endl;
    }
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Build the 'recent files' menu
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    QMenu* recentFilesMenu = ui->menuBar->findChild<QMenu *>("menuRecent_Files");

    if (recentFilesMenu != NULL) {
        for (int i = 0; i < MaxRecentFiles; ++i)
            recentFilesMenu->addAction(recentFileActs[i]);
    }

    // Reload the recent file menu
    updateRecentFileActions();

    // Reload the 'windows' menu
    updateWindowMenu();

    ui->progressBar->setValue(0);
    QString config = QDir::currentPath()+"/miracle.config";
    QFile f(config);
    if(f.exists(config))
    {
        ui->lineEditConfigFile->setText(config);
    }
    string title = "Miracle-Grue ";
    title += mgl::getMiracleGrueVersionStr().c_str();
    this->setWindowTitle(title.c_str());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_GCode_triggered()
{

    QString fileName;
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "",//"/home",
                                                   tr("3D Models (*.stl);;GCode (*.gcode)") // tr("GCode (*.gcode)")
                                                    );
    }
    if(fileName.size())
    {
        GCodeViewApplication::LoadFile(fileName);
        setCurrentFile(fileName);
    }
}

// TODO: Move this to the app class
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowFilePath(curFile);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    // TODO: Disable 'recent files' menu if there aren't any.
//    separatorAct->setVisible(numRecentFiles > 0);
}

void MainWindow::updateWindowMenu()
{
    QMenu* windowMenu = ui->menuBar->findChild<QMenu *>("menuWindow");

    if (windowMenu) {
        // TODO: Does this cause a memory leak?
        windowMenu->clear();

        // Re-add the windows
        foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
            if (mainWin) {
                QAction* action = new QAction(this);
                action->setText(mainWin->windowTitle());
                windowMenu->addAction(action);
 //               windowMenu->addAction(recentFileActs[i]);
            }
        }
    }
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        GCodeViewApplication::LoadFile(action->data().toString());
}



bool MainWindow::hasFile() {
    return ui->graphicsView->hasModel();
}

void MainWindow::on_LayerHeight_sliderMoved(int position)
{
    // TODO: where /should/ this signal go?
    ui->graphicsView->setMaximumVisibleLayer(position);
    ui->LayerMin->setMaximum(ui->graphicsView->model.getMapSize() );

    // display the current layer height.
}

void MainWindow::on_LayerMin_valueChanged(int value)
{
    ui->graphicsView->setMinimumVisibleLayer(value);
}

void MainWindow::on_zoomIn_clicked()
{
    ui->graphicsView->zoom(1.1);
}

void MainWindow::on_zoomOut_clicked()
{
    ui->graphicsView->zoom(.9);
}

void MainWindow::on_actionClose_triggered()
{
    // Close this window.
    this->close();

    // TODO: Can the application get a signal when this happens, instead of sending it explicitly here?
}


void MainWindow::on_panLeft_clicked()
{
    ui->graphicsView->panX(1.0);
}

void MainWindow::on_panRight_clicked()
{
    ui->graphicsView->panX(-1.0);
}

void MainWindow::on_panUp_clicked()
{
    ui->graphicsView->panY(-1.0);
}

void MainWindow::on_panDown_clicked()
{
    ui->graphicsView->panY(1.0);
}

void MainWindow::on_buttonConfigBrowse_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Miracle-Grue configurations (*.config)") );
    ui->lineEditConfigFile->setText(fileName);

}

//void MainWindow::on_button3dModelBrowse_clicked()
//{
//    QString fileName;

//    string modelFile = ui->lineEdit3dModelFile->text().toStdString();

//    MyComputer computer;
//    string dir = computer.fileSystem.ExtractDirectory(modelFile.c_str());

//    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(dir.c_str()), tr("3D models (*.stl)") );

//    ui->lineEdit3dModelFile->setText(fileName);
//    sliceModelAndCreateToolPaths(fileName.toAscii());
//}


void MainWindow::sliceModelAndCreateToolPaths(const char* modelpath)
{
    try
    {
        cout << "Output file: ";
        MyComputer computer;

        string configFileName = ui->lineEditConfigFile->text().toStdString();
        string modelFile = modelpath; //ui->lineEdit3dModelFile->text().toStdString();

        string gcodeFile = computer.fileSystem.ChangeExtension(computer.fileSystem.ExtractFilename(modelFile.c_str()).c_str(), ".gcode" );
        cout << gcodeFile << endl;
        string scadFile = computer.fileSystem.ChangeExtension(computer.fileSystem.ExtractFilename(modelFile.c_str()).c_str(), ".scad" );
        cout << scadFile << endl;

        //configFileName += "/miracle.config";
        mgl::Configuration config;
        cout << "loading config: " << configFileName << endl;
        config.readFromFile(configFileName.c_str());

        GCoderConfig gcoderCfg;
        loadGCoderConfigFromFile(config, gcoderCfg);
        SlicerConfig slicerCfg;
        loadSlicerConfigFromFile(config, slicerCfg);
		RegionerConfig regionerCfg;
		loadRegionerConfigFromFile(config, regionerCfg);
		ExtruderConfig extruderCfg;
		loadExtruderConfigFromFile(config, extruderCfg);

        int shells = ui->spinBoxShells->value();

        double density = atof(ui->lineEditDensity->text().toAscii());
        regionerCfg.infillDensity = density;

        regionerCfg.nbOfShells = shells;
        cout << "Shells  " << regionerCfg.nbOfShells << endl;
        cout << "Density " << regionerCfg.infillDensity << endl;

        Progress progress(ui->label_task, ui->progressBar);

        Tomograph tomograph; 	// = ui->graphicsView->model.tomograph;
		LayerLoops layerloops;
        RegionList regions; 		//  = ui->graphicsView->model.regions;
        std::vector<mgl::SliceData> slices;
		LayerPaths layerpaths;

		Meshy mesh;
		mesh.readStlFile(modelFile.c_str());
		mesh.alignToPlate();

		Segmenter segmenter(slicerCfg.firstLayerZ, slicerCfg.layerH, 
				slicerCfg.layerW);
		segmenter.tablaturize(mesh);

        Slicer slicer(slicerCfg, &progress);
		slicer.generateLoops(segmenter, layerloops);
        cout << "Slicer done" << endl;

        Regioner regioner(regionerCfg, &progress);
        regioner.generateSkeleton(layerloops, regions);
        cout << "Regioner done" << endl;

        Pather pather(&progress);
        pather.generatePaths(extruderCfg, layerloops, regions, layerpaths);
        cout << "Pather done" << endl;

        ui->graphicsView->loadSliceData(layerloops, regions, layerpaths);
        ui->LayerHeight->setMaximum(ui->graphicsView->model.getMapSize());
        ui->LayerMin->setMaximum(ui->graphicsView->model.getMapSize());
        ui->LayerMin->setValue(0);
        ui->LayerHeight->setValue(0);

        // things we need to remember
        ui->graphicsView->model.layerMeasure = tomograph.layerMeasure;
        ui->graphicsView->model.slices = slices;
		ui->graphicsView->model.slicePaths = layerpaths;
        ui->graphicsView->model.gcoderCfg = gcoderCfg;
        ui->graphicsView->model.modelFile = modelFile;
    }
    catch(mgl::Exception &mixup)
    {
        cout << "ERROR: " << mixup.error << endl;
        QMessageBox &box = *new QMessageBox();
        box.setText(mixup.error.c_str());
        box.show();
    }
    catch(...)
    {
         cout << "ERROR" << endl;
    }
}

void MainWindow::loadFile(const QString &fileName)
{
    setWindowTitle(strippedName(fileName));


    QAction* closeMenu = ui->menuBar->findChild<QAction *>("actionClose");
    if (closeMenu) {
        closeMenu->setText("Close \"" + fileName + "\"");
    }

    std::string x(fileName.toAscii());
    
    size_t dot_index = x.find_last_of('.');
    size_t x_size = x.size();

    std::string ex = x.substr(dot_index, x_size-1);

    if( (ex == ".stl") ||  (ex == ".STL") )
    {
        cout << "STL!!!" << ex << endl;
        sliceModelAndCreateToolPaths(fileName.toAscii());
    }
    else
    {
        ui->graphicsView->loadModel(fileName);
        ui->LayerHeight->setMaximum(ui->graphicsView->model.getMapSize() );
        ui->LayerMin->setMaximum(ui->graphicsView->model.getMapSize() );
        ui->LayerMin->setValue(0 );
        ui->LayerHeight->setValue(0 );

    }
}


void MainWindow::on_actionSave_gcode_triggered()
{
    Progress progress(ui->label_task, ui->progressBar);
    cout << "hello on_actionSave_gcode_triggered save menu!" << endl;
    QString filename = QFileDialog::getSaveFileName(this, tr("Export GCode"), QDir::currentPath(),  tr("GCode File (*.gcode)"));
    cout << "SAVE into " << filename.toStdString() << endl;
    ui->graphicsView->exportModel(filename, &progress);
}

void MainWindow::on_checkBoxSurfs_toggled(bool checked)
{
     cout << "Toggle surfs" << endl;
     ui->graphicsView->toggleSurfs(checked);
}

void MainWindow::on_checkBoxRoofs_toggled(bool checked)
{
     cout << "Toggle roofs " <<checked << endl;
     ui->graphicsView->toggleRoofs(checked);
}

void MainWindow::on_checkBoxFloors_toggled(bool checked)
{
    cout << "Toggle floors " <<checked << endl;
    ui->graphicsView->toggleFLoors(checked);
}

void MainWindow::on_checkBoxLoops_toggled(bool checked)
{
    cout << "Toggle loops " <<checked << endl;
    ui->graphicsView->toggleLoops(checked);
}

void MainWindow::on_checkBoxInfills_toggled(bool checked)
{
    cout << "Toggle infills " <<checked << endl;
    ui->graphicsView->toggleInfills(checked);
}

void MainWindow::on_actionOpen_3D_model_triggered()
{
    cout << "OPEN!" << endl;
    QString fileName;
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "",//"/home",
                                                   tr("GCode (*.stl)") //  tr("3D Models (*.stl);;GCode (*.gcode)")
                                                    );
    }
    if(fileName.size())
    {
        GCodeViewApplication::LoadFile(fileName);
        setCurrentFile(fileName);
    }
}

void MainWindow::on_pushButtonSlice_clicked()
{
    sliceModelAndCreateToolPaths(ui->graphicsView->model.modelFile.c_str());
}
