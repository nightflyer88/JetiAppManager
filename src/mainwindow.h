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
    void doDownload(const QUrl &url);
    static bool isHttpRedirect(QNetworkReply *reply);

    ~MainWindow();

    void debugLogPrint(QtMsgType type, const QMessageLogContext &context, const QString &msg);

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

private:
    Ui::MainWindow *ui;

    enum{
        sourceAppInfoFile=1,
        descriptionfile,
        previewIcon,
        sourcefile
    };

    struct appInfo{
        QString appName;
        int fileType;
    };
    QMap<QString, appInfo> urlFile;


    struct appData{
        QString author;
        QString version;
        int minHW = unknownTransmitter;
        float minSW = 0;
        QPixmap previewImg;
        QString description;
        QStringList sourceFile;
        QStringList destinationPath;
    };
    QMap<QString, appData> luaApp;

    Document appDescription;

    QString getCurrentAppName();

    bool isTransmitterValid(QString rootpath);

    bool isAppInstalled(QString appName);

    void updateAppList();

    void getSourceList();

    void updateAppStatus();

    struct transmitterData{
        int typHW = unknownTransmitter;
        float versionSW = 0;
        QString driveName;
        QString rootPath;
    };

    QList<transmitterData> jetiTransmitter;

    transmitterData getDevice(transmitterData device);

    QStringList getVolumes();


public slots:
    void downloadFinished(QNetworkReply *reply);

private slots:
    void updateAppDescription();

    void on_actionPreferences_triggered();

    void on_buttonInstall_clicked();

    void on_buttonUninstall_clicked();

    void jetiVolume_changed(int index);

    void loadSettings();

    void checkMountedVolumes();

};

#endif // MAINWINDOW_H
