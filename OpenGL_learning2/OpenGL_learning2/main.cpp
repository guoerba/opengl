#include <QApplication>
#include "openglwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    OpenGLWindow w;
    w.resize(600,600);
    w.show();

    return a.exec();
}
