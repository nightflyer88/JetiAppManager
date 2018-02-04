#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    a.setOrganizationName("Nightflyer88");
    a.setOrganizationDomain("https://github.com/nightflyer88");
    a.setApplicationName("Jeti App Manager");

    QSettings settings;
    QString language = ":/language/" + settings.value("language", DEFAULT_LANGUAGE).toString();

    QTranslator translator;
    translator.load(language);
    a.installTranslator(&translator);


    MainWindow w;
    w.show();

    return a.exec();
}
