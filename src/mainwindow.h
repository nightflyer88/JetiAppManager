#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "document.h"
#include "preferences.h"
#include "defaults.h"
#include "appmanager.h"
#include "listdelegate.h"

#include <QMessageBox>
#include <QSettings>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include <QFile>
#include <QFileInfo>

#include <QMap>
#include <QStringList>

#include <QtNetwork>
#include <QUrl>
#include <QWebChannel>

#include <QListWidgetItem>
#include <QRegExp>

#include <stdio.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void debugLogPrint(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    Ui::MainWindow *ui;

    Document appDescription;

    AppManager *appManager = new AppManager(this);

    QString getCurrentAppName();

    int getCurrentTransmitter();

    void getAppInformation();

private slots:
    void on_app_clicked();

    void on_actionPreferences_triggered();

    void on_buttonInstall_clicked();

    void on_buttonUninstall_clicked();

    void on_searchText_textChanged(const QString &arg1);

    void on_actionHelp_triggered();

    void jetiVolume_changed(int index);

    void loadSettings();

    void checkMountedVolumes();

    void reloadAppList(QRegExp regExp = QRegExp("", Qt::CaseInsensitive, QRegExp::Wildcard));

    void reloadAppDescription(QString file);

    void reloadAppStatus();

};

#endif // MAINWINDOW_H
