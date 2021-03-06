#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "defaults.h"
#include "appmanager.h"
#include "preferences.h"
#include "listdelegate.h"
#include "document.h"
#include "appinfo.h"

#include <QMessageBox>
#include <QSettings>

#include <QDebug>
#include <QWebChannel>

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

    bool show_newApps;

    bool show_appUpdates;

    QString getCurrentAppName();

    int getCurrentTransmitter();

    void getAppInformation();

private slots:
    void app_clicked();

    void on_actionPreferences_triggered();

    void on_buttonInstall_clicked();

    void on_buttonUninstall_clicked();

    void on_searchText_textChanged(const QString &arg1);

    void on_actionHelp_triggered();

    void newAppAvailable(QStringList newApps);

    void appUpdateAvailable(QStringList appUpdates);

    void jetiVolume_changed(int index);

    void loadSettings();

    void checkMountedVolumes();

    void checkForAppUpdates();

    void reloadAppList(QRegExp regExp = QRegExp("", Qt::CaseInsensitive, QRegExp::Wildcard));

    void reloadAppDescription(QString file);

    void reloadAppStatus();

};

#endif // MAINWINDOW_H
