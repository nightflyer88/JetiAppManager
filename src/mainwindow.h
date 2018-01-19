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
        QPixmap previewImg;
        QString description;
        QStringList sourceFile;
        QStringList destinationPath;
    };
    QMap<QString, appData> luaApp;

    Document appDescription;

    QStringList getVolumes();

    QString currentApp = "";

    void updateAppList();

    void getSourceList();



public slots:
    void downloadFinished(QNetworkReply *reply);

private slots:
    void itemPressed();

    void on_actionPreferences_triggered();

    void on_buttonInstall_clicked();

};

#endif // MAINWINDOW_H
