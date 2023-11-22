#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion"); //color scheme
    MainWindow w;
    w.show();
    return a.exec();
}
