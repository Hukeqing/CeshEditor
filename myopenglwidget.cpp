#include "myopenglwidget.h"
#include "color.h"
#include "QMenuBar"
#include <QOpenGLShaderProgram>
#include <QHBoxLayout>
#include <algorithm>
#include <QMessageBox>
#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include "defination.h"
// 1.0
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    zoom_in = 45.0f;
    near_plane = 0.1f;
    far_plane = 1000.0f;
    addVerticeButton = nullptr;
    addIndiceButton = nullptr;
    lastMousePoint.setX(-1);

    rotationSlider = new QSlider(this);
    rotationSlider->setMinimum(-180);
    rotationSlider->setMaximum(180);
    rotationSlider->setSingleStep(1);
    rotationSlider->setOrientation(Qt::Horizontal);
    connect(rotationSlider, &QSlider::valueChanged, this, &MyOpenGLWidget::meshRotate);

    pitchSlider = new QSlider(this);
    pitchSlider->setMinimum(-60);
    pitchSlider->setMaximum(60);
    pitchSlider->setSingleStep(1);
    pitchSlider->setOrientation(Qt::Vertical);
    connect(pitchSlider, &QSlider::valueChanged, this, &MyOpenGLWidget::meshRotate);

    zoomScroll = new QSlider(this);
    zoomScroll->setMinimum(1);
    zoomScroll->setMaximum(179);
    zoomScroll->setSingleStep(1);
    zoomScroll->setOrientation(Qt::Horizontal);
    zoomScroll->setValue(45);
    connect(zoomScroll, &QSlider::valueChanged, this, &MyOpenGLWidget::meshRotate);

    verticeSetDialog = new verticeDialog(this);
    this->setMinimumSize(QSize(500, 300));
    connect(verticeSetDialog, &verticeDialog::ok, this, &MyOpenGLWidget::verticeSet);
    connect(verticeSetDialog, &verticeDialog::delete_cmd, this, &MyOpenGLWidget::deleteVertice);
    connect(verticeSetDialog, &verticeDialog::add, this, &MyOpenGLWidget::addVertice);

    indiceSetDialog = new indiceDialog(this);
    this->setMinimumSize(QSize(500, 300));
    connect(indiceSetDialog, &indiceDialog::ok, this, &MyOpenGLWidget::indiceSet);
    connect(indiceSetDialog, &indiceDialog::delete_cmd, this, &MyOpenGLWidget::deleteindice);
    connect(indiceSetDialog, &indiceDialog::add, this, &MyOpenGLWidget::addindice);

    // 2.0
    rightMenu = new QMenu(this);
//    rightMenu->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_showButton = rightMenu->addAction(QString("EditorMode"));
    action_showButton->setCheckable(true);
    action_showButton->setChecked(true);
    QAction *action_triangleMode = rightMenu->addAction(QString("showSurface"));
    action_triangleMode->setCheckable(true);
    action_triangleMode->setChecked(true);
    QAction *action_showGrid = rightMenu->addAction(QString("showGrid"));
    action_showGrid->setCheckable(true);
    action_showGrid->setChecked(true);
    rightMenu->addSeparator();
    QAction *action_save = rightMenu->addAction(QString("Save\tCtrl+S"));
    QAction *action_save_as = rightMenu->addAction(QString("Save as..."));
    QAction *action_open = rightMenu->addAction(QString("Open"));
    rightMenu->addSeparator();
    QAction *action_reset = rightMenu->addAction(QString("reset\tCtrl+R"));
    QAction *action_reCube = rightMenu->addAction(QString("recube"));
    QAction *action_clear = rightMenu->addAction(QString("clear\tCtrl+C"));
    rightMenu->addSeparator();
    QAction *action_exit = rightMenu->addAction(QString("Exit"));
    connect(action_showButton, &QAction::triggered, this, &MyOpenGLWidget::showButton);
    connect(action_triangleMode, &QAction::triggered, this, [&](bool isShow) {surfaceMode = isShow == false ? GL_LINE_STRIP : GL_TRIANGLES; update();});
    connect(action_showGrid, &QAction::triggered, this, [&](bool isShow) {this->gridMode = isShow; update();});
    connect(action_save, &QAction::triggered, this, &MyOpenGLWidget::saveCesh);
    connect(action_save_as, &QAction::triggered, this, &MyOpenGLWidget::saveAs);
    connect(action_open, &QAction::triggered, this, &MyOpenGLWidget::open);
    connect(action_reset, &QAction::triggered, this, &MyOpenGLWidget::reset);
    connect(action_reCube, &QAction::triggered, this, &MyOpenGLWidget::cube);
    connect(action_clear, &QAction::triggered, this, &MyOpenGLWidget::clear);
    connect(action_exit, &QAction::triggered, this, [&](){this->close();});

    onEditorMode = true;
    onMouseMove = false;
    gridMode = true;
    surfaceMode = GL_TRIANGLES;
    saveFile.clear();
    isSaved = false;

    this->setMinimumSize(QSize(800, 600));
    this->setTitle();
}

void MyOpenGLWidget::meshRotate()
{
    if (onMouseMove)
        return;
    mesh.transform.rotation.y = rotationSlider->value();
    grid.transform.rotation.y = rotationSlider->value();
    mesh.transform.rotation.x = pitchSlider->value();
    grid.transform.rotation.x = pitchSlider->value();
    mesh.transform.apply();
    grid.transform.apply();
//    double dist = 3;
//    double rad = pitchSlider->value() / 180.0 * acos(-1);

//    camera.position.z = GLfloat((1 - cos(rad)) * dist);
//    camera.position.y = -GLfloat(sin(rad) * dist);
//    camera.position.y = -GLfloat(tan(rad) * dist);
//    camera.rotation.x = pitchSlider->value();
//    camera.apply();
    zoom_in = zoomScroll->value();
    update();
}

void MyOpenGLWidget::verticeButton()
{
    QPushButton *send = qobject_cast<QPushButton *>(sender());
    size_t index = 0;
    for (; index < verticeButtonVector.size(); ++index)
        if (send == verticeButtonVector[index])
            break;
    verticeSetDialog->set(int(index), mesh.get_vertice(GLuint(index)));
    verticeSetDialog->exec();
}

void MyOpenGLWidget::verticeAddButton()
{
    verticeSetDialog->set(-1, nullptr);
    verticeSetDialog->exec();
}

void MyOpenGLWidget::verticeSet(int index, float x, float y, float z, float r, float g, float b)
{
    GLfloat newVertice[6] = {x, y, z, r / 255, g / 255, b / 255};
    mesh.change_vertice(GLuint(index), newVertice);
    reGUI_vertice(GLuint(index));
    update();
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::addVertice(float x, float y, float z, float r, float g, float b)
{
    GLfloat newVertice[6] = {x, y, z, r / 255, g / 255, b / 255};
    mesh.push_vertice(newVertice);
    add_verticeGUI();
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::deleteVertice(int index)
{
    int maxindice = -1;
    for (auto &item : mesh.get_indice())
        maxindice = std::max(maxindice, int(item));
    if (maxindice < int(mesh.get_vertice_number() - 1)) {
        mesh.erase_vertice(GLuint(index));
        delete verticeButtonVector[size_t(index)];
        verticeButtonVector.erase(verticeButtonVector.begin() + index);
        for (GLuint i = GLuint(index); i < mesh.get_vertice_number(); ++i)
            reGUI_vertice(i);
        resizeGL(width(), height());
        this->isSaved = false;
        setTitle();
    } else {
        QMessageBox::critical(this, "Error!", "the max of indices is the same as the lengh of vertices!", QMessageBox::Ok);
    }
}

void MyOpenGLWidget::indiceButton()
{
    QPushButton *send = qobject_cast<QPushButton *>(sender());
    size_t index = 0;
    for (; index < indiceButtonVector.size(); ++index)
        if (send == indiceButtonVector[index])
            break;
    indiceSetDialog->set(int(index), mesh.get_indice(GLuint(index)));
    indiceSetDialog->exec();
}

void MyOpenGLWidget::indiceAddButton()
{
    indiceSetDialog->set(-1, nullptr);
    indiceSetDialog->exec();
}

void MyOpenGLWidget::indiceSet(int index, unsigned int a, unsigned int b, unsigned int c)
{
    if (a >= mesh.get_vertice_number() ||
        b >= mesh.get_vertice_number() ||
        c >= mesh.get_vertice_number()) {
        QMessageBox::critical(this, "Error!", "the max of indices is larger than the lengh of vertices!", QMessageBox::Ok);
        return;
    }
    if (a == b || a == c || b == c) {
        QMessageBox::critical(this, "Error!", "there is two vertice is same!", QMessageBox::Ok);
        return;
    }
    GLuint newIndice[3] = {GLuint(a), GLuint(b), GLuint(c)};
    mesh.change_indice(GLuint(index), newIndice);
    reGUI_indice(GLuint(index));
    update();
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::addindice(unsigned int a, unsigned int b, unsigned int c)
{
    if (a >= mesh.get_vertice_number() ||
        b >= mesh.get_vertice_number() ||
        c >= mesh.get_vertice_number()) {
        QMessageBox::critical(this, "Error!", "the max of indices is larger than the lengh of vertices!", QMessageBox::Ok);
        return;
    }
    if (a == b || a == c || b == c) {
        QMessageBox::critical(this, "Error!", "there is two vertice is same!", QMessageBox::Ok);
        return;
    }
    GLuint newIndice[3] = {GLuint(a), GLuint(b), GLuint(c)};
    mesh.push_indice(newIndice);
    add_indiceGUI();
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::deleteindice(int index)
{
    mesh.erase_indice(GLuint(index));
    delete indiceButtonVector[size_t(index)];
    indiceButtonVector.erase(indiceButtonVector.begin() + index);
    for (GLuint i = GLuint(index); i < mesh.get_indice_number(); ++i)
        reGUI_indice(i);
    resizeGL(width(), height());
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();
    // 创建着色器程序
    mesh.init();
    cube();
    //    mesh.transform.rotate(Vector3(0, -60, -60));
    mesh.transform.translate(Vector3(0, 0, 3));

    grid.init();
    gridinit();
    grid.transform.translate(Vector3(0, 0, 3));
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MyOpenGLWidget::paintGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    int w = width();
    int h = height();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 projection, view;
    projection.perspective(zoom_in, GLfixed(w) / GLfloat(h), near_plane, far_plane);
    camera.get_view_matrix(view);
    mesh.draw(projection, view);
    glDrawElements(surfaceMode, int(mesh.get_indices_len()), GL_UNSIGNED_INT, mesh.get_indice_data());
//    glDrawElements(GL_LINES, int(mesh.get_indices_len()), GL_UNSIGNED_INT, mesh.get_indice_data());
    mesh.undraw();
    if (gridMode) {
        grid.draw(projection, view);
        glDrawElements(GL_LINES, int(grid.get_indices_len()), GL_UNSIGNED_INT, grid.get_indice_data());
        grid.undraw();
    }
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    int hei = std::min(60, (h - 50) / int(mesh.get_vertice_number() + 1));
    for (GLuint i = 0; i < verticeButtonVector.size(); ++i) {
        QPushButton *curButton = verticeButtonVector[i];
        curButton->setGeometry(0, int(i) * hei, 200, hei);
        curButton->setVisible(onEditorMode);
    }
    addVerticeButton->setGeometry(0, int(verticeButtonVector.size()) * hei, 50, hei / 2);
    addVerticeButton->setVisible(onEditorMode);
    hei = std::min(60, h / int(mesh.get_indice_number() + 1));
    for (GLuint i = 0; i < indiceButtonVector.size(); ++i) {
        QPushButton *curButton = indiceButtonVector[i];
        curButton->setGeometry(w - 100, int(i) * hei, 100, hei);
        curButton->setVisible(onEditorMode);
    }
    addIndiceButton->setGeometry(w - 50, int(indiceButtonVector.size()) * hei, 50, hei / 2);
    addIndiceButton->setVisible(onEditorMode);
    rotationSlider->setGeometry(0, h - 50, 100, 20);
    pitchSlider->setGeometry(100, h - 50, 20, 50);
    zoomScroll->setGeometry(0, h - 20, 100, 20);
}

void MyOpenGLWidget::GUI()
{
    for (GLuint i = 0; i < GLuint(mesh.get_vertice_number()); ++i) {
        QPushButton *newButton = new QPushButton(this);
        newButton->setText(QString::number(i) + ": " + mesh.get_vertice_position_name(i));
        newButton->setFont(QFont("Fira Code"));
        newButton->setStyleSheet(mesh.get_vertice_css(i));
        newButton->show();
        connect(newButton, &QPushButton::clicked, this, &MyOpenGLWidget::verticeButton);
        verticeButtonVector.push_back(newButton);
    }
    if (addVerticeButton == nullptr) {
        addVerticeButton = new QPushButton(this);
        addVerticeButton->setText("New");
        addVerticeButton->setFont(QFont("Fira Code"));
        addVerticeButton->show();
    }
    connect(addVerticeButton, &QPushButton::clicked, this, &MyOpenGLWidget::verticeAddButton);

    for (GLuint i = 0; i < GLuint(mesh.get_indice_number()); ++i) {
        QPushButton *newButton = new QPushButton(this);
        newButton->setText(QString::number(i) + ": " + mesh.get_indice_name(i));
        newButton->setFont(QFont("Fira Code"));
//        newButton->setStyleSheet(mesh.get_indice_3colorCup(i));
        newButton->show();
        connect(newButton, &QPushButton::clicked, this, &MyOpenGLWidget::indiceButton);
        indiceButtonVector.push_back(newButton);
    }
    if (addIndiceButton == nullptr) {
        addIndiceButton = new QPushButton(this);
        addIndiceButton->setText("New");
        addIndiceButton->setFont(QFont("Fira Code"));
        addIndiceButton->show();
    }
    connect(addIndiceButton, &QPushButton::clicked, this, &MyOpenGLWidget::indiceAddButton);
}

void MyOpenGLWidget::reGUI_vertice(GLuint index)
{
    int h = height();
    int hei = std::min(60, (h - 50) / int(mesh.get_vertice_number() + 1));
    QPushButton *newButton = verticeButtonVector[index];
    newButton->setText(QString::number(index) + ": " + mesh.get_vertice_position_name(index));
    newButton->setStyleSheet(mesh.get_vertice_css(index));
    newButton->setGeometry(0, int(index) * hei, 200, hei);
}

void MyOpenGLWidget::add_verticeGUI()
{
    int h = height();
    int hei = std::min(60, (h - 50) / int(mesh.get_vertice_number() + 1));
    GLuint i = GLuint(verticeButtonVector.size());
    QPushButton *newButton = new QPushButton(this);
    newButton->setText(QString::number(i) + ": " + mesh.get_vertice_position_name(i));
    newButton->setFont(QFont("Fira Code"));
    newButton->setStyleSheet(mesh.get_vertice_css(i));
    newButton->setGeometry(0, int(i) * hei, 200, hei);
    connect(newButton, &QPushButton::clicked, this, &MyOpenGLWidget::verticeButton);
    newButton->show();
    verticeButtonVector.push_back(newButton);
    addVerticeButton->setGeometry(0, int(i + 1) * hei, 50 ,hei / 2);
}

void MyOpenGLWidget::reGUI_indice(GLuint index)
{
    int w = width();
    int h = height();
    int hei = std::min(60, h / int(mesh.get_indice_number() + 1));
    QPushButton *newButton = indiceButtonVector[index];
    newButton->setText(QString::number(index) + ": " + mesh.get_indice_name(index));
//    newButton->setStyleSheet(mesh.get_indice_3colorCup(index));
    newButton->setGeometry(w - 100, int(index) * hei, 100, hei);
}

void MyOpenGLWidget::add_indiceGUI()
{
    int w = width();
    int h = height();
    int hei = std::min(60, h / int(mesh.get_indice_number() + 1));
    GLuint i = GLuint(indiceButtonVector.size());
    QPushButton *newButton = new QPushButton(this);
    newButton->setText(QString::number(i) + ": " + mesh.get_indice_name(i));
    newButton->setFont(QFont("Fira Code"));
//    newButton->setStyleSheet(mesh.get_indice_3colorCup(i));
    newButton->setGeometry(w - 100, int(i) * hei, 100, hei);
    connect(newButton, &QPushButton::clicked, this, &MyOpenGLWidget::indiceButton);
    newButton->show();
    indiceButtonVector.push_back(newButton);
    addIndiceButton->setGeometry(w - 50, int(i + 1) * hei, 50, hei / 2);
}
// 2.0
void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        rightMenu->popup(event->globalPos());
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && event->x() > 200 && event->x() < width() - 100) {
        onMouseMove = true;
        if (lastMousePoint.x() != -1) {
//            mesh.transform.rotate(Vector3(0, 0.3f, 0) * (event->x() - lastMousePoint.x()) * cos(radians(mesh.transform.rotation.x)));
//            grid.transform.rotate(Vector3(0, 0.3f, 0) * (event->x() - lastMousePoint.x()) * cos(radians(mesh.transform.rotation.x)));
            mesh.transform.rotate(Vector3(0, 0.3f, 0) * (event->x() - lastMousePoint.x()));
            grid.transform.rotate(Vector3(0, 0.3f, 0) * (event->x() - lastMousePoint.x()));
//            mesh.transform.rotate(Vector3(0, 0, -0.3f) * (event->x() - lastMousePoint.x()) * sin(radians(mesh.transform.rotation.x)));
//            grid.transform.rotate(Vector3(0, 0, -0.3f) * (event->x() - lastMousePoint.x()) * sin(radians(mesh.transform.rotation.x)));
            mesh.transform.rotate(Vector3(-0.3f, 0, 0) * (event->y() - lastMousePoint.y()));
            grid.transform.rotate(Vector3(-0.3f, 0, 0) * (event->y() - lastMousePoint.y()));
            if (mesh.transform.rotation.x > 60) {
                mesh.transform.rotation.x = 60;
                grid.transform.rotation.x = 60;
            }
            else if (mesh.transform.rotation.x < -60) {
                mesh.transform.rotation.x = -60;
                grid.transform.rotation.x = -60;
            }
            rotationSlider->setValue(int(mesh.transform.rotation.y));
            pitchSlider->setValue(int(mesh.transform.rotation.x));
        }
        lastMousePoint = event->pos();
    }
    update();
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePoint.setX(-1);
    }
}

void MyOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->x() > 200 && event->x() < width() - 100) {
        //    qDebug() << event->angleDelta();
        zoom_in -= event->angleDelta().y() / 24;
        if (zoom_in > 179)
            zoom_in = 179;
        else if (zoom_in < 1)
            zoom_in = 1;
        zoomScroll->setValue(int(zoom_in));
        update();
    }
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_R:
            reset();
            break;
        case Qt::Key_C:
            clear();
            break;
        case Qt::Key_S:
            saveCesh();
            break;
        }
    } else if (event->key() == Qt::Key_Escape) {
        this->close();
    } else {
        QOpenGLWidget::keyPressEvent(event);
    }
}

void MyOpenGLWidget::showButton(bool isShow)
{
    for (auto &item : verticeButtonVector)
        item->setVisible(isShow);
    addVerticeButton->setVisible(isShow);
    for (auto &item : indiceButtonVector)
        item->setVisible(isShow);
    addIndiceButton->setVisible(isShow);
    rotationSlider->setVisible(isShow);
    pitchSlider->setVisible(isShow);
    zoomScroll->setVisible(isShow);
    onEditorMode = isShow;
}

void MyOpenGLWidget::reset()
{
    zoom_in = 45.0f;
    zoomScroll->setValue(45);
    mesh.transform.rotation = Vector3(0, 0, 0);
    grid.transform.rotation = Vector3(0, 0, 0);
    mesh.transform.apply();
    grid.transform.apply();
    rotationSlider->setValue(0);
    update();
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::clear()
{
    mesh.clear();
    for (auto &item : verticeButtonVector)
        delete item;
    for (auto &item : indiceButtonVector)
        delete item;
    verticeButtonVector.clear();
    indiceButtonVector.clear();
    resizeGL(width(), height());
    update();
    this->isSaved = false;
    setTitle();
}

void MyOpenGLWidget::cube()
{
    mesh.clear();
    for (auto &item : verticeButtonVector)
        delete item;
    for (auto &item : indiceButtonVector)
        delete item;
    verticeButtonVector.clear();
    indiceButtonVector.clear();
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
    mesh.push_vertice(ver, 8);
    mesh.push_indice(ind, 12);
    GUI();
    resizeGL(width(), height());
    update();
}

void MyOpenGLWidget::gridinit() {
    GLfloat ver[] = { 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
                      0.0f,  0.0f, 10.0f, 0.0f, 0.0f, 1.0f,
                      0.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 10.0f,  0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
                     10.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
                    -10.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
                      0.0f,-10.0f,  0.0f, 1.0f, 1.0f, 1.0f,
                      0.0f,  0.0f,-10.0f, 1.0f, 1.0f, 1.0f};
    GLuint ind[] = {0, 1, 2, 3, 4, 5, 6, 7, 6, 8, 6, 9};
    grid.push_vertice(ver, 10);
    grid.push_indice(ind, 6, 2);
}

void MyOpenGLWidget::saveCesh()
{
    if (this->saveFile.size() == 0)
        saveFile = QFileDialog::getSaveFileName(this, QString("Save as a cesh file"), QString("."), QString("Cesh File(*.cesh)"));
    if (saveFile.size() == 0)
        return;
    QFile file(saveFile);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, QString("Error"), QString("open file fail!\nPlease try again"));
        saveFile.clear();
        file.close();
        return;
    }
    QTextStream out(&file);
    mesh.writeCesh(out);
    this->isSaved = true;
    setTitle();
}

void MyOpenGLWidget::saveAs()
{
    QString tempFile = QFileDialog::getSaveFileName(this, QString("Save as another file"), QString("."), QString("Cesh File(*.cesh);;Obj File(*.obj)"));
    if (tempFile.size() == 0)
        return;
    QFile file(tempFile);
    if (tempFile.section('.', -1) == QString("obj"))
        QMessageBox::information(this, QString("Hint"), QString("Save as a obj file will lose color information!"));
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, QString("Error"), QString("save error!\nPlease try again"));
        saveFile.clear();
        file.close();
        return;
    }
    QTextStream out(&file);
    mesh.writeObj(out);
}

void MyOpenGLWidget::open()
{
    QString tempFile = QFileDialog::getOpenFileName(this, QString("Open"), QString("."), QString("Cesh File(*.cesh);;Obj File(*.obj)"));
    if (tempFile.section('.', -1) == QString("cesh")) {
        QFile file(tempFile);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, QString("Error"), QString("save error!\nPlease try again"));
            saveFile.clear();
            file.close();
            return;
        }
        QTextStream in(&file);
        mesh.loadCesh(in);
    }
    GUI();
    resizeGL(width(), height());
    this->isSaved = true;
    setTitle();
}

void MyOpenGLWidget::setTitle()
{
    QString title;
    if (!this->isSaved)
        title += "*";
    if (this->saveFile.size() == 0)
        title += "New Cesh";
    else {
        title += saveFile;
    }
    title += "- CeshEditor" + QString::number(VERSION >> 8);
    setWindowTitle(title);
}
