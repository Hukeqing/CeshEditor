#include "myopenglwidget.h"
#include <QOpenGLShaderProgram>
#include <QHBoxLayout>
#include <algorithm>
#include <QMessageBox>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    zoom_in = 45.0f;
    near_plane = 0.1f;
    far_plane = 1000.0f;
    verticeSetDialog = new verticeDialog(this);
    this->setMinimumSize(QSize(300, 200));
    connect(verticeSetDialog, &verticeDialog::ok, this, &MyOpenGLWidget::verticeSet);
    connect(verticeSetDialog, &verticeDialog::delete_cmd, this, &MyOpenGLWidget::deleteVertice);
}

void MyOpenGLWidget::verticeButton()
{
    QPushButton *send = qobject_cast<QPushButton *>(sender());
    size_t index = 0;
    for (; index < verticeButtonVector.size(); ++index)
        if (send == verticeButtonVector[index])
            break;
    verticeSetDialog->set(int(index), mesh.get_vertice(GLuint(index)));
    verticeSetDialog->show();
}

void MyOpenGLWidget::verticeSet(int index, float x, float y, float z, float r, float g, float b)
{
    GLfloat newVertice[6] = {x, y, z, r / 255, g / 255, b / 255};
    mesh.change_vertice(GLuint(index), newVertice);
    re_verticeGUI(GLuint(index));
    update();
}

void MyOpenGLWidget::deleteVertice(int index)
{
    GLuint maxindice = 0;
    for (auto &item : mesh.get_indice())
        maxindice = std::max(maxindice, item);
    if (maxindice < mesh.get_vertice().size() / 6 - 1) {
        mesh.erase_vertice(GLuint(index));
        delete verticeButtonVector[size_t(index)];
        verticeButtonVector.erase(verticeButtonVector.begin() + index);
        for (GLuint i = GLuint(index); i < GLuint(verticeButtonVector.size()); ++i) {
            re_verticeGUI(i);
        }
    } else {
        QMessageBox::critical(this, "Error!", "the max of indices is the same as the lengh of vertices!", QMessageBox::Ok);
    }
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
        ,1, 1, 1, 1, 1, 1
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
    mesh.init();
    mesh.push_vertice(ver, 9);
    mesh.push_indice(ind, 12);
    mesh.transform.rotate(Vector3(0, -60, -60));
    mesh.transform.translate(Vector3(0, 0, 3));
    GUI();
}

void MyOpenGLWidget::resizeGL(int , int )
{
    int w = width();
    int h = height();
    int hei = std::min(60, h / int(mesh.get_vertice().size()) * 6);
    for (GLuint i = 0; i < verticeButtonVector.size(); ++i) {
        QPushButton *curButton = verticeButtonVector[i];
        curButton->setGeometry(0, int(i) * hei, 200, hei);
    }
    hei = std::min(60, h / int(mesh.get_indice().size()) * 3);
    for (GLuint i = 0; i < indiceButtonVector.size(); ++i) {
        QPushButton *curButton = indiceButtonVector[i];
        curButton->setGeometry(w - 100, int(i) * hei, 100, hei);
    }
}

void MyOpenGLWidget::GUI()
{
//    for (auto &item : verticeButtonVector)
//        item->close();
//    for (auto &item : indiceButtonVector)
//        item->close();
//    verticeButtonVector.clear();
//    indiceButtonVector.clear();
    for (GLuint i = 0; i < GLuint(mesh.get_vertice().size() / 6); ++i) {
        QPushButton *newButton = new QPushButton(this);
        newButton->setText(QString::number(i) + ": " + mesh.get_vertice_position_name(i));
        newButton->setFont(QFont("Fira Code"));
        newButton->setStyleSheet(mesh.get_vertice_css(i));
        connect(newButton, &QPushButton::clicked, this, &MyOpenGLWidget::verticeButton);
        verticeButtonVector.push_back(newButton);
    }
    for (GLuint i = 0; i < GLuint(mesh.get_indice().size() / 3); ++i) {
        QPushButton *newButton = new QPushButton(this);
        newButton->setText(mesh.get_indice_name(i));
        newButton->setFont(QFont("Fira Code"));
        indiceButtonVector.push_back(newButton);
    }
//    qDebug() << indiceButtonVector[0]->pos();
}

void MyOpenGLWidget::re_verticeGUI(GLuint index)
{
    QPushButton *newButton = verticeButtonVector[index];
    newButton->setText(QString::number(index) + ": " + mesh.get_vertice_position_name(index));
    newButton->setFont(QFont("Fira Code"));
    newButton->setStyleSheet(mesh.get_vertice_css(index));
    newButton->setGeometry(0, int(index) * 60, 200, 60);
}

void MyOpenGLWidget::add_verticeGUI()
{
    for (GLuint i = GLuint(verticeButtonVector.size()); i < GLuint(mesh.get_vertice().size() / 6); ++i) {
        QPushButton *newButton = new QPushButton(this);
        newButton->setText(mesh.get_vertice_position_name(i));
        newButton->setFont(QFont("Fira Code"));
        newButton->setStyleSheet(mesh.get_vertice_css(i));
        newButton->setGeometry(0, int(i) * 60, 200, 60);
        verticeButtonVector.push_back(newButton);
    }
}

void MyOpenGLWidget::paintGL()
{
    glGetError();
    glEnable(GL_DEPTH_TEST);
    int w = width();
    int h = height();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 projection, view;
    projection.perspective(zoom_in, GLfixed(w) / GLfloat(h), near_plane, far_plane);
    camera.get_view_matrix(view);
    mesh.draw(projection, view);

    glDrawElements(GL_TRIANGLES, int(mesh.get_indices_len()), GL_UNSIGNED_INT, mesh.get_indice_data());
}
