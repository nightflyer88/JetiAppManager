#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // this is necessary for support of deserialization of the preference data for linux
    // if this is missing an error message is given and no URLs are loaded:
    // > QVariant::load: unknown user type with name QList<QString>
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
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
