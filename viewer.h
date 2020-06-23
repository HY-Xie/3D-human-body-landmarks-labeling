#ifndef VIEWER_H
#define VIEWER_H

#include <QGLViewer/qglviewer.h>
#include <Eigen/Dense>
#include <vector>
class Viewer: public QGLViewer
{
    Q_OBJECT
public:

    virtual void draw();
    virtual void init();
    virtual QString helpString() const;
    virtual void postSelection(const QPoint &point);
    virtual void keyPressEvent(QKeyEvent *e);
    qglviewer::Vec orig, dir, selectedVec;

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::MatrixXd N;

    std::vector<int> selectedIndices;
    std::vector<qglviewer::Vec> selectedPoints;
    bool meshHasLoaded;

    void clearAll();

    void drawMesh();
    void drawLandMarks();
    void drawSelectedPoints();



signals:
    void orderSignal(int);
};

#endif // VIEWER_H
