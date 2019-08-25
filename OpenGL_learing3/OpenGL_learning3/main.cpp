#include <QApplication>
#include "openglwindow.h"

int main(int argc, char *argv[])
{
   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QGuiApplication a(argc, argv);

    OpenGLWindow w;
    w.show();

    return a.exec();
}
