#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QPushButton>
#include <QSlider>
#include <QMenu>
#include <QFileDialog>
#include "mesh.h"
#include "verticedialog.h"
#include "indicedialog.h"
#include <QFile>

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    // 1.0
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
public slots:
    void meshRotate();

    void verticeButton();
    void verticeAddButton();
    void verticeSet(int index, float x, float y, float z, float r, float g, float b);
    void addVertice(float x, float y, float z, float r, float g, float b);
    void deleteVertice(int index);

    void indiceButton();
    void indiceAddButton();
    void indiceSet(int index, unsigned int a, unsigned int b, unsigned int c);
    void addindice(unsigned int a, unsigned int b, unsigned int c);
    void deleteindice(int index);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    Mesh mesh;
    Transform camera;
    GLfloat zoom_in, near_plane, far_plane;
    // transfrom
    QSlider *rotationSlider, *zoomScroll, *pitchSlider;
    // vertice Editor
    vector<QPushButton *> verticeButtonVector, indiceButtonVector;
    QPushButton *addVerticeButton, *addIndiceButton;
    verticeDialog *verticeSetDialog;
    indiceDialog *indiceSetDialog;
    void GUI();
    void reGUI_vertice(GLuint index);
    void add_verticeGUI();
    void reGUI_indice(GLuint index);
    void add_indiceGUI();
    // 2.0
private:
    // right menu
    QMenu *rightMenu;
    QPoint lastMousePoint;
    // grid
    Mesh grid;
    // onEditorMode: is show other thing like button...
    // onMouseMove: is the mouse on move, so there is no need to care about the slider
    // gridMode: is show the grid
    bool onEditorMode, onMouseMove, gridMode;
    // surface show mode : triangle or lines
    unsigned int surfaceMode;
    // save and load
    QString saveFile;
    bool isSaved;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
public slots:
    void showButton(bool isShow);
    void reset();
    void clear();
    void cube();
    void gridinit();
    // save and load
    void saveCesh();
    void saveAs();
    void setTitle();
};

#endif // MYOPENGLWIDGET_H
