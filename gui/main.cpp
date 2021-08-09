#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Mesh Value Transfer Test");
    QApplication::setApplicationVersion("1.0");

    QSurfaceFormat fmt = QVTKOpenGLNativeWidget::defaultFormat();
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
//    w.show();
    w.showMaximized();
    return a.exec();
}
