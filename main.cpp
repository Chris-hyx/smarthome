#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
    w.setWindowTitle("智能家居登陆");
    w.show();
    return a.exec();
}
