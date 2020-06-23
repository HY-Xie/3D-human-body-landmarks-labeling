#include "viewer.h"
#include <QDebug>
#include <vector>
#include <QMessageBox>
#include <GL/freeglut.h>
bool isPointInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d C);
void Viewer::draw()
{
    if(meshHasLoaded)
    {
        drawMesh();
        //        if(selectedPoints.size() != 0)
        //            drawSelectedPoints();
        if(selectedIndices.size() != 0)
            drawLandMarks();
    }
}

void Viewer::init()
{
    restoreStateFromFile();
    glClearColor(1.f, 1.f, 1.f, 1.f);
    setKeyDescription(Qt::Key_Z|Qt::ControlModifier ,tr("撤销"));
}

QString Viewer::helpString() const {
    QString text("<h2>三维人体标注程序</h2>");
    text += "总共需要选择23个关键点, 按界面左侧提示<b>依次</b>选取\n";
    text += "按下鼠标左键旋转视角查看\n";
    text += "滑动鼠标滚轮进行缩放\n";
    text += "按下<b>Shift+鼠标左键</b>进行点选\n";
    text += "按下<b>Ctrl+Z</b>撤销之前选择的关键点\n";
    text += "一个模型的关键点选择完成后,直接点击保存即可.若未选择全部的关键点,无法保存.\n";
    text += "================================================================";
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the "
            "three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera "
            "view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys "
            "(<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several "
            "keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and "
            "restored at next start.<br><br>";
    text +=
            "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save "
            "a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut "
            "list.<br><br>";
    text += "Double clicks automates single click actions: A left button double "
            "click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the "
            "right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed "
            "defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for "
            "details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}

void Viewer::postSelection(const QPoint &point)
{
    camera()->convertClickToLine(point, orig, dir);
    bool found;
    selectedVec = camera()->pointUnderPixel(point, found);
    qDebug() << "Selected Vec: " << found << " " << selectedVec.x << " " << selectedVec.y << " " << selectedVec.z;
    if(found)
    {   selectedPoints.push_back(selectedVec);

        Eigen::Vector3d selectedPoint(selectedVec.x, selectedVec.y, selectedVec.z);
        std::vector<float> distances; distances.clear();
        for(int i = 0; i < V.rows(); ++i)
        {
            Eigen::Vector3d v(V.row(i));
            distances.push_back((selectedPoint*100-v*100).norm());
        }
        auto iter = std::min_element(distances.begin(), distances.end());
        int idx = std::distance(distances.begin(), iter);

        if(selectedIndices.size() > 22)
            return;
        selectedIndices.push_back(idx);

        emit orderSignal(selectedIndices.size());

    }
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Z  &&  e->modifiers() == Qt::ControlModifier)
    {   if(selectedIndices.size() != 0)
        {
            selectedIndices.pop_back();
            emit orderSignal(selectedIndices.size());
        }

    }
    else
    {
        QGLViewer::keyPressEvent(e);
    }

    update();

}

void Viewer::clearAll()
{
    meshHasLoaded = false;
    selectedIndices.clear();
    selectedPoints.clear();

}

void Viewer::drawMesh()
{
    glColor3f(0.8, 0.8, 0.8);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < F.rows(); ++i)
    {
        int idx0 = F(i, 0);
        int idx1 = F(i, 1);
        int idx2 = F(i, 2);
        glNormal3f(N(i, 0), N(i, 1), N(i, 2));
        glVertex3f(V(idx0, 0), V(idx0, 1), V(idx0, 2));
        glVertex3f(V(idx1, 0), V(idx1, 1), V(idx1, 2));
        glVertex3f(V(idx2, 0), V(idx2, 1), V(idx2, 2));
    }
    glEnd();
    glPopMatrix();
}

void Viewer::drawLandMarks()
{
    glColor3f(1.f, 0.f, 0.f);
    for(auto it = selectedIndices.begin(); it != selectedIndices.end(); ++it)
    {
        glPushMatrix();
        glTranslatef(V.row(*it).x(), V.row(*it).y(), V.row(*it).z());
        glutSolidSphere(0.015, 30, 30);
        glPopMatrix();
    }
}

void Viewer::drawSelectedPoints()
{
    glColor3f(1.f, 1.f, 0.f);
    for(auto it = selectedPoints.begin(); it != selectedPoints.end(); ++it)
    {
        glPushMatrix();
        glTranslatef(it->x, it->y, it->z);
        glutSolidSphere(0.015, 30, 30);
        glPopMatrix();
    }
}




bool isPointInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d C)
{
    using namespace Eigen;
    Vector3d v0 = C-A;
    Vector3d v1 = B-A;
    Vector3d v2 = P-A;
    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);

    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

    float u = (dot11*dot02 - dot01*dot12) * inverDeno;
    if(u < 0 || u > 1)
        return false;
    float v = (dot00 * dot12 - dot01*dot02) * inverDeno;
    if(v < 0 || v > 1)
        return false;
    return u+v <= 1;
}
