#include "myopenglwidget.h"
#include <QOpenGLShaderProgram>
#include <QPushButton>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
//    QPushButton *push = new QPushButton(this);
//    push->setText("abc");
//    push->setFixedSize(QSize(100, 50));
}

void MyOpenGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();
    // 创建着色器程序
    GLfloat ver[] = { 0.5,  0.5,  0.5, 1.0, 0.0, 0.0,
                      0.5,  0.5, -0.5, 1.0, 0.0, 0.0,
                      0.5, -0.5,  0.5, 0.0, 1.0, 0.0,
                      0.5, -0.5, -0.5, 0.0, 1.0, 0.0,
                     -0.5, -0.5,  0.5, 0.0, 0.0, 1.0,
                     -0.5, -0.5, -0.5, 0.0, 0.0, 1.0,
                     -0.5,  0.5,  0.5, 1.0, 0.0, 1.0
                     -0.5,  0.5, -0.5, 1.0, 0.0, 1.0};
    GLuint ind[] = {0, 2, 6,
                    2, 4, 6,
                    1, 2, 0,
                    1, 3, 2,
                    1, 5, 3,
                    1, 7 ,5,
                    5, 4, 2,
                    2, 3, 5,
                    4, 5, 7,
                    4, 7, 6,
                    0, 7, 6,
                    0, 1, 7};
    mesh.init(ver, 48, QString("vcm"), ind, 36);
}

void MyOpenGLWidget::resizeGL(int , int )
{
}

void MyOpenGLWidget::paintGL()
{
    glGetError();
    glEnable(GL_DEPTH_TEST);
    int w = width();
    int h = height();
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh.draw(w, h);

    glDrawElements(GL_TRIANGLES, int(mesh.getIndicesLen()), GL_UNSIGNED_INT, mesh.getIndices());
}
