#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <igl/readOBJ.h>
#include <igl/readOFF.h>
#include <igl/per_face_normals.h>

#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGUI();
    initLabels();
    viewer = new Viewer();
    ui->horizontalLayout_11->addWidget(viewer);

    meshFolder = QDir::homePath();
    connect(viewer, &Viewer::orderSignal, this, &MainWindow::orderIdxSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGUI()
{
    initActions();
    initMenus();
    initToolBar();
    initStatusBar();
}

void MainWindow::initMenus()
{
    filesMenu = menuBar()->addMenu(tr("Files"));
    filesMenu->addAction(loadMeshAction);
    filesMenu->addAction(saveLandmarksAction);
}

void MainWindow::initActions()
{
    loadMeshAction = new QAction(tr("Load Mesh"));
    connect(loadMeshAction, &QAction::triggered, this, &MainWindow::loadMeshSlot);

    saveLandmarksAction = new QAction(tr("Save Landmarks"));
    connect(saveLandmarksAction, &QAction::triggered, this, &MainWindow::saveLandmarksSlot);
}

void MainWindow::initToolBar()
{
    ui->mainToolBar->addAction(loadMeshAction);
    ui->mainToolBar->addAction(saveLandmarksAction);
}

void MainWindow::initStatusBar()
{
    ui->statusBar->showMessage(tr("按H查看帮助信息. 左右方向按模型左右为准(非观测者).完成后直接点击保存即可,自动存储在模型所在目录."));
}

void MainWindow::loadMeshSlot()
{
    meshPathName = QFileDialog::getOpenFileName(this, "Open Mesh",
                                                meshFolder,
                                                tr("Mesh (*.obj *.off)"));
    if(meshPathName.isEmpty())
        return;
//    meshPathName = "/home/haoyang/Desktop/Final_Exps/mesh000_1/mesh000_1.obj";

    // Obtain ext name
    QStringList fileNameList = meshPathName.split(".");
    QString ext = fileNameList[fileNameList.size()-1];
    // Obtain mesh Name
    fileNameList = meshPathName.split("/");
    meshName = fileNameList[fileNameList.size()-1].split(".")[0];
    meshFolder = meshPathName.mid(0, meshPathName.size()-4-meshName.size());
    if(ext == "obj" || ext == "OBJ")
    {
        if(!igl::readOBJ(meshPathName.toStdString(), viewer->V, viewer->F))
        {
            QMessageBox::information(this, "Error", "Failed to load obj mesh");
            return;
        }

    }
    else if(ext == "off" || ext == "OFF")
    {
        if(!igl::readOFF(meshPathName.toStdString(), viewer->V, viewer->F))
        {
            QMessageBox::information(this, "Error", "Failed to load off mesh");
            return;
        }
    }
    viewer->clearAll();
    igl::per_face_normals(viewer->V, viewer->F, Eigen::Vector3d(1,1,1).normalized(), viewer->N);

    viewer->meshHasLoaded = true;
    viewer->update();


    orderIdxSlot(0);

    ui->statusBar->showMessage("Current Mesh: " + meshName + "." + ext);

//    qDebug() << "Load Mesh Slot Done";
}

void MainWindow::saveLandmarksSlot()
{
    if(viewer->selectedIndices.size() != 23)
    {
        QMessageBox::information(this, "Error", "You should select 23 landmarks");
        return;
    }
    QString fullName = meshFolder + meshName + "_landmarks.txt";
    std::ofstream file(fullName.toStdString());
    if(file.is_open())
    {
        for(auto it = viewer->selectedIndices.begin(); it != viewer->selectedIndices.end(); ++it)
        {
            file << *it << std::endl;
        }
        file.close();
    }
    QString msg = "Landmarks file has been saved in " + fullName;
    QMessageBox::information(this, "Successfully", msg);
}

void MainWindow::orderIdxSlot(int idx)
{

    if(idx > 23)
    {
        return;
    }
    if(idx == 23)
        idx = 22;
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::gray);
    for(int i = 0; i < labels.size(); ++i)
    {
        labels[i]->setPalette(pe);
    }
    pe.setColor(QPalette::WindowText, Qt::red);
    labels[idx]->setPalette(pe);
}

void MainWindow::initLabels()
{
    labels.clear();

    labels.push_back(ui->left_neck);
    labels.push_back(ui->right_neck);
    labels.push_back(ui->front_neck);
    labels.push_back(ui->left_acromion);
    labels.push_back(ui->right_acromion);
    labels.push_back(ui->left_armpit);
    labels.push_back(ui->right_armpit);
    labels.push_back(ui->left_elbow);
    labels.push_back(ui->right_elbow);
    labels.push_back(ui->left_wrist);
    labels.push_back(ui->right_wrist);
    labels.push_back(ui->left_nipple);
    labels.push_back(ui->right_nipple);
    labels.push_back(ui->high_waist);
    labels.push_back(ui->waist);
    labels.push_back(ui->low_waist);
    labels.push_back(ui->crotch);
    labels.push_back(ui->left_knee);
    labels.push_back(ui->right_knee);
    labels.push_back(ui->left_ankle);
    labels.push_back(ui->right_ankle);
    labels.push_back(ui->left_hip);
    labels.push_back(ui->right_hip);
}
