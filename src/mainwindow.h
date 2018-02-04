#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "document.h"
#include "preferences.h"
#include "defaults.h"
#include "listdelegate.h"

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
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;

public:
    explicit MainWindow(QWidget *parent = 0);

    enum{
        appInfoFile=1,
        descriptionfile,
        previewIcon,
        sourcefile
    };

    enum transmitterTyp{
        unknownTransmitter=0,
        DS24 = 0x02A7,
        DC24 = 0x02A6,
        DC14 = 0x02A5,
        DS14 = 0x02A4,
        DS16 = 0x02A3,
        DC16 = 0x02A2,
    };
    Q_ENUM(transmitterTyp)

    void doDownload(const QUrl &url, QString appName, int fileType);

    static bool isHttpRedirect(QNetworkReply *reply);

    ~MainWindow();

    void debugLogPrint(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    Ui::MainWindow *ui;

    struct appData{
        bool doInstall = false;
        bool doUninstall = false;
        QString author;
        QString version;
        QPixmap previewImg;
        QString description;
        float requiredFirmware = 0;
        QStringList sourceFile;             //source files for all transmitters
        QStringList sourceFile14_16;        //source files only for DC/DS 14,16
        QStringList sourceFile24;           //source files only for DC/DS 24
        QStringList destinationPath;
    };
    QMap<QString, appData> luaApp;

    Document appDescription;

    bool error = false;

    QString getCurrentAppName();

    int getCurrentTransmitter();

    bool isTransmitterValid(QString rootpath);

    bool isTransmitterSupportLua(QString rootpath);

    bool isTransmitterSupportApp(int hwTyp, QString appName);

    bool isTransmitter14_16(int hwTyp);

    bool isTransmitter24(int hwTyp);

    bool isAppInstalled(QString appName);

    void updateAppList(QRegExp regExp = QRegExp("", Qt::CaseInsensitive, QRegExp::Wildcard));

    void getSourceList();

    void updateAppStatus();

    struct transmitterData{
        int transmitterTyp = unknownTransmitter;
        float firmwareVersion = 0;
        QString driveName;
        QString rootPath;
    };

    QList<transmitterData> jetiTransmitter;

    transmitterData getDevice(transmitterData device);

    QStringList getVolumes();

    QStringList getAppSourceList(int hwTyp, appData app);


public slots:
    void downloadFinished(QNetworkReply *reply);

private slots:
    void on_app_clicked();

    void on_actionPreferences_triggered();

    void on_buttonInstall_clicked();

    void on_buttonUninstall_clicked();

    void jetiVolume_changed(int index);

    void loadSettings();

    void checkMountedVolumes();

    void on_searchText_textChanged(const QString &arg1);

    void on_actionHelp_triggered();
};

#endif // MAINWINDOW_H
