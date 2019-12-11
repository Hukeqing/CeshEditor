#include <QApplication>
#include "myopenglwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    MyOpenGLWidget w;
    w.resize(1200, 900);
    w.show();
    return app.exec();
}
