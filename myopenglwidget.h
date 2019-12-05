#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QPushButton>
#include "mesh.h"

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
public slots:
    void verticeButton();
//    void verticeSet(int index, float x, float y, float z, float r, float g, float b);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void GUI();
    void re_verticeGUI(GLuint index);
    void add_verticeGUI();
private:
    Mesh mesh;
    Transform camera;
    GLfloat zoom_in, near_plane, far_plane;
    vector<QPushButton *> verticeButtonVector, indiceButtonVector;
};

#endif // MYOPENGLWIDGET_H
