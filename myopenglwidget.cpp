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
    GLfloat ver[] = {// front
                     -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,     // 0 RT
                     -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,     // 1 RB
                      0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,     // 2 LB
                      0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,     // 3 LT
                     // back
                     -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f,     // 4 RT
                     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,     // 5 RB
                      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,     // 6 LB
                      0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f      // 7 LT
                    };
    GLuint ind[] = {0, 1, 3,
                    1, 2, 3,
                    4, 5, 7,
                    5, 6, 7,
                    3, 2, 7,
                    2, 6, 7,
                    0, 1, 4,
                    1, 5, 4,
                    0, 7, 3,
                    0, 4, 7,
                    1, 6, 2,
                    1, 5, 6};
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh.draw(w, h);

    glDrawElements(GL_TRIANGLES, int(mesh.getIndicesLen()), GL_UNSIGNED_INT, mesh.getIndices());
}
