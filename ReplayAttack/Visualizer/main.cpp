#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);
    MainWindow w;
#ifdef Q_OS_ANDROID
    w.showFullScreen();
#else
    w.show();
#endif

    return a.exec();
}
