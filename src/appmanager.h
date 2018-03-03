#ifndef APPMANAGER_H
#define APPMANAGER_H

#include "defaults.h"

#include <QDebug>
#include <QSettings>
#include <QJsonDocument>
#include <QFile>
#include <QtNetwork>
#include <QListWidgetItem>

class AppManager : public QObject
{
    Q_OBJECT

public:
    AppManager(QObject *parent);

    enum files{
        appInfofile=1,
        descriptionfile,
        previewIcon,
        installSourcefile,
        sha1FileCheck
    };

    struct App{
        QString name;
        QString version;
        QString author;
        QString previewImgURL;
        QPixmap previewImg;
        QString description;
        float requiredFirmware = 0;
        QStringList sourceFile;             //source files for all transmitters
        QStringList sourceFile14_16;        //source files only for DC/DS 14,16
        QStringList sourceFile24;           //source files only for DC/DS 24
        QStringList destinationPath;
        bool isInstalled = false;
        bool isNew = false;
        bool updateAvailable = false;
    };
    QMap<QString, App> applist;

    enum HWID{
        DS_24 = 0x02A7,
        DC_24 = 0x02A6,
        DC_14 = 0x02A5,
        DS_14 = 0x02A4,
        DS_16 = 0x02A3,
        DC_16 = 0x02A2,
    };

    struct Transmitter{
        int device = 0;
        float firmwareVersion = 0;
        QString driveName;
        QString rootPath;
    };
    QList<Transmitter> transmitterList;

    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;

    void doDownload(const QUrl &url, QString appName, int fileType, QByteArray oldFileHash = 0, bool printDebug = true);

    void downloadAppInformation(QList<QString> sourcelist);

    void getAppDescription(QString appName);

    void checkAllAppsForUpdate(Transmitter transmitter);

    void installApp(Transmitter transmitter, QString appName);

    bool uninstallApp(Transmitter transmitter, QString appName);

    bool isTransmitterSupportApp(Transmitter transmitter, QString appName);

    bool isTransmitter14_16(int hwTyp);

    bool isTransmitter24(int hwTyp);

    bool isAppInstalled(Transmitter transmitter, QString appName);

    Transmitter getTransmitterDevice(Transmitter transmitter);

    QString getTransmitterName(Transmitter transmitter);

    QStringList transmitterVolumes();

    void setCurrentTransmitterIndex(int index);

    QStringList getAppSourceList(Transmitter transmitter, App app);

    QString getLocalLanguage();

    void saveNewApps();


private:

    bool error = false;

    int currentTransmitterIndex = 0;

    bool appInformationIsLoaded = false;

    int countDownload_appInfofile;

    int countDownload_installSourcefile;

    int countDownload_sha1FileCheck;

    static bool isHttpRedirect(QNetworkReply *reply);

    bool isTransmitterValid(QString rootpath);

    bool isTransmitterSupportLua(QString rootpath);

    bool encodeAppInformation(QString file);

    App encodeAppValues(QJsonObject value, App app);


public slots:

    virtual void close();


private slots:

    void downloadFinished(QNetworkReply *reply);


signals:

    void hasNewApp(QStringList newApps);

    void hasNewAppInformation();

    void hasNewAppStatus();

    void hasNewAppDescription(QString file);

    void hasNewAppUpdate(QStringList appUpdate);

    void appInformationIsDownloaded();

};

#endif // APPMANAGER_H
