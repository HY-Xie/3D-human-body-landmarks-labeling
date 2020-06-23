#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewer.h"
#include <vector>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


public:
    Viewer *viewer;

    QMenu *filesMenu;
    QAction *loadMeshAction;
    QAction *saveLandmarksAction;

    void initGUI();
    void initMenus();
    void initActions();
    void initToolBar();
    void initStatusBar();

    void loadMeshSlot();
    void saveLandmarksSlot();


    QString meshPathName;
    QString meshFolder;
    QString meshName;

    int orderIdx;
    void orderIdxSlot(int idx);

    std::vector<QLabel*> labels;
    void initLabels();


};

#endif // MAINWINDOW_H
