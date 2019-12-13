#include <QApplication>
#include "myopenglwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    MyOpenGLWidget w;
//    w.showFullScreen();
    w.showMaximized();
    w.show();
    return app.exec();
}
