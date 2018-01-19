#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Nightflyer88");
    a.setOrganizationDomain("https://github.com/nightflyer88");
    a.setApplicationName("Jeti App Manager");


    MainWindow w;
    w.show();

    return a.exec();
}
