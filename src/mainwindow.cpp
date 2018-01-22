#include <QCoreApplication>

#include "mainwindow.h"
#include "previewpage.h"
#include "ui_mainwindow.h"


namespace logOutput {

QPointer<MainWindow> activeDialog;

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

// Redirector to the current active dialog
void debugLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(activeDialog.isNull())
        return;

    activeDialog->debugLogPrint(type, context, msg);

    // Call the default handler.
    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

void Register(QPointer<MainWindow> dialog) {
    if(!activeDialog.isNull())
        return;

    activeDialog = dialog;
    qInstallMessageHandler(&logOutput::debugLogOutput);
}

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // register qDebug() to output log
    logOutput::Register(this);

    qDebug() << "### Start Jeti App Manager ###";

    ui->buttonUninstall->setHidden(true);
    ui->buttonInstall->setHidden(true);

    // search volume
    QTimer *searchVolume = new QTimer(this);
    connect(searchVolume, SIGNAL(timeout()), this, SLOT(checkMountedVolumes()));
    searchVolume->start(1000);

    connect(ui->jetiVolume, SIGNAL(currentIndexChanged(int)), this, SLOT(jetiVolume_changed(int)));


    // setup app list
    connect(ui->appList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
        this, SLOT(updateAppDescription()));

    // setup md-file viewer
    PreviewPage *page = new PreviewPage(this);
    ui->description->setPage(page);

    appDescription.setText("## Jeti App Manager\n"
                           "Wähle eine LUA-App aus der Liste");

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &appDescription);
    page->setWebChannel(channel);

    ui->description->setUrl(QUrl("qrc:/index.html"));

    // setup downloadmanager
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
                SLOT(downloadFinished(QNetworkReply*)));

    // get settings
    loadSettings();

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::checkMountedVolumes()
{
    static int mountedVolumes = 0;
    if(mountedVolumes != QStorageInfo::mountedVolumes().length()){
        mountedVolumes = QStorageInfo::mountedVolumes().length();
        jetiTransmitter.clear();
        ui->jetiVolume->clear();
        ui->jetiVolume->addItems(getVolumes());
    }
}

QStringList MainWindow::getVolumes()
{

    QStringList volumes;

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if(!storage.isReadOnly() && isTransmitterValid(storage.rootPath())){
                transmitterData readDevice;
                readDevice.driveName = storage.displayName();
                readDevice.rootPath = storage.rootPath();
                readDevice = getDevice(readDevice);
                jetiTransmitter.append(readDevice);

                volumes.append(storage.displayName());
                qDebug("Jeti transmitter found: Type: %i Firmware: %.2f",readDevice.typHW, readDevice.versionSW);
            }
        }
    }

    return volumes;
}

bool MainWindow::isTransmitterValid(QString rootpath)
{
    QString destFile = TRANSMITTER_CONFIGFILE;
    #ifdef __APPLE__
        // OSX path
        destFile = rootpath + destFile;
    #elif _WIN32
        // WIN path
        destFile = rootpath + destFile.right(destFile.size()-1);
    #endif

    QFileInfo check_file(destFile);
    // check if config file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    }

    return false;
}

MainWindow::transmitterData MainWindow::getDevice(MainWindow::transmitterData device)
{
    QFile configfile(device.rootPath + TRANSMITTER_CONFIGFILE);

    if(configfile.open(QIODevice::ReadOnly))
    {
        QTextStream instream(&configfile);
        QString file = instream.readAll();
        configfile.close();

        // encode config file
        QJsonDocument config = QJsonDocument::fromJson(file.toUtf8());
        if(config.isObject()){
            QJsonObject value = config.object();

            if (value.contains("version") && value["version"].isString())
                device.versionSW = value["version"].toString().toFloat();

            if (value.contains("hw") && value["hw"].isDouble())
                device.typHW = value["hw"].toDouble();

        }

    }else{
        qDebug() << "ERROR: no config file on transmitter found !";
    }

    return device;
}

bool MainWindow::isAppInstalled(QString appName)
{

    if(luaApp.contains(appName)){
        appData app = luaApp.value(appName);

        for(int i=0;i<app.sourceFile.size();i++){
            QUrl url = QUrl::fromEncoded(app.sourceFile[i].toLocal8Bit());

            QString destFile;
            #ifdef __APPLE__
                // OSX path
                destFile = jetiTransmitter[ui->jetiVolume->currentIndex()].rootPath + app.destinationPath[i] + "/" + url.fileName();
            #elif _WIN32
                // WIN path
                destFile = jetiTransmitter[ui->jetiVolume->currentIndex()].rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1) + "/" + url.fileName();
            #endif

            QFileInfo check_file(destFile);
            // check if file exists and if yes: Is it really a file and no directory?
            if (check_file.exists() && check_file.isFile()) {
                return true;
            }
        }

    }

    return false;
}


void MainWindow::updateAppList()
{
    // print apps in list
    ui->appList->clear();

    QStringList list = luaApp.keys();
    std::sort(list.begin(), list.end());

    ui->appList->setItemDelegate(new ListDelegate(ui->appList));

    for(int i = 0; i < list.size(); i++){
        appData app = luaApp.value(list[i]);

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, list[i]);
        item->setData(Qt::UserRole + 1, app.version);
        item->setData(Qt::UserRole + 2, app.author);
        item->setData(Qt::DecorationRole, app.previewImg);
        ui->appList->addItem(item);

    }

}

void MainWindow::getSourceList()
{
    ui->statusBar->showMessage("Lade App Informationen...");

    // read sources from settings
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QSettings settings;

    QList<QString> defUrl;
    defUrl << DEFAULT_SOURCE_URL;
    QList<QString> sourcelist = settings.value("sourceList",QVariant::fromValue(defUrl)).value<QList<QString>>();

    // download sources
    QString source;
    foreach( source, sourcelist ){
        QUrl url = QUrl::fromEncoded(source.toLocal8Bit());
        appInfo appinfo;
        appinfo.fileType = sourceAppInfoFile;
        urlFile.insert(url.fileName(), appinfo);
        doDownload(url);
    }
}

QString MainWindow::getCurrentAppName()
{
    QModelIndex index = ui->appList->currentIndex();
    return index.data(Qt::DisplayRole).toString();
}

void MainWindow::jetiVolume_changed(int index)
{

    if(index >= 0){
        QMetaEnum metaEnum = QMetaEnum::fromType<MainWindow::transmitterTyp>();
        ui->type->setText(metaEnum.valueToKey(jetiTransmitter[index].typHW));
        ui->firmware->setText(QString::number(jetiTransmitter[index].versionSW));
    }else{
        ui->type->setText("???");
        ui->firmware->setText("???");
    }

    updateAppStatus();

}

void MainWindow::updateAppDescription()
{
    // show app description
    QModelIndex index = ui->appList->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();

    appData app;

    if(luaApp.contains(itemText)){
        app = luaApp.value(itemText);

        // show description
        QUrl url = QUrl::fromEncoded(app.description.toLocal8Bit());
        if(url.isValid()){
            ui->statusBar->showMessage("Lade Beschreibung...");
            appInfo appinfo;
            appinfo.fileType = descriptionfile;
            urlFile.insert(url.fileName(), appinfo);
            doDownload(url);
        }else{
            appDescription.setText("keine App Beschreibung verfügbar");
        }

        updateAppStatus();
    }
}

void MainWindow::updateAppStatus()
{

    if(!jetiTransmitter.isEmpty()){
        ui->buttonInstall->setHidden(false);

        if(isAppInstalled(getCurrentAppName())){
            ui->buttonInstall->setStyleSheet("background-color: rgb(15, 128, 255)");
            ui->buttonInstall->setText("Aktualisieren");
            ui->buttonUninstall->setStyleSheet("background-color: rgb(252, 1, 7)");
            ui->buttonUninstall->setHidden(false);
        }else{
            ui->buttonInstall->setStyleSheet("background-color: rgb(128, 255, 7)");
            ui->buttonInstall->setText("Installieren");
            ui->buttonUninstall->setHidden(true);
        }

    }else{
        ui->buttonInstall->setHidden(true);
        ui->buttonUninstall->setHidden(true);
    }

    ui->statusBar->clearMessage();
}

void MainWindow::doDownload(const QUrl &url)
{
    qDebug() << "download file:" << url.toString();

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    currentDownloads.append(reply);
}

bool MainWindow::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}


void MainWindow::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        qDebug("ERROR: Download of %s failed: %s",url.toEncoded().constData(),qPrintable(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            qDebug("ERROR: Request was redirected");
        } else {
            QIODevice *data = reply;

            if (urlFile.contains(url.fileName())){

                appInfo appinfo = urlFile.value(url.fileName());

                if(appinfo.fileType == sourceAppInfoFile){

                    QString file = data->readAll();

                    // encode source file
                    QJsonDocument source = QJsonDocument::fromJson(file.toUtf8());
                    if(source.isObject()){
                        QJsonObject appName = source.object();

                        foreach (const QString& name, appName.keys()) {
                            QJsonObject value = appName[name].toObject();
                            appData app;

                            if (value.contains("author") && value["author"].isString())
                                app.author = value["author"].toString();

                            if (value.contains("version") && value["version"].isString())
                                app.version = value["version"].toString();

                            if (value.contains("previewImg") && value["previewImg"].isString()){
                                QUrl urlImg = QUrl::fromEncoded(value["previewImg"].toString().toLocal8Bit());
                                if(urlImg.isValid()){
                                    appInfo iconinfo;
                                    iconinfo.appName = name;
                                    iconinfo.fileType = previewIcon;
                                    urlFile.insert(urlImg.fileName(), iconinfo);
                                    doDownload(urlImg);
                                }
                            }


                            if (value.contains("description") && value["description"].isString())
                                app.description = value["description"].toString();

                            if (value.contains("sourceFile") && value["sourceFile"].isArray()){
                                QJsonArray sourceFileArray = value["sourceFile"].toArray();

                                foreach (const QJsonValue& sourcefile, sourceFileArray) {
                                    app.sourceFile.append(sourcefile.toString());
                                }
                            }

                            if (value.contains("destinationPath") && value["destinationPath"].isArray()){
                                QJsonArray destinationPathArray = value["destinationPath"].toArray();

                                foreach (const QJsonValue& destinationpath, destinationPathArray) {
                                    app.destinationPath.append(destinationpath.toString());
                                }
                            }

                            luaApp.insert(name, app);
                            updateAppList();
                        }

                    }

                }else if(appinfo.fileType == descriptionfile){

                    QString file = data->readAll();

                    // set app description
                    appDescription.setText(file);

                }else if(appinfo.fileType == previewIcon){

                    // load preview icon
                    appData curApp = luaApp.value(appinfo.appName);
                    curApp.previewImg.loadFromData(data->readAll());
                    luaApp[appinfo.appName] = curApp;

                    updateAppList();

                }else if(appinfo.fileType == sourcefile){

                    // copy file to transmitter
                    appData curApp = luaApp.value(appinfo.appName);

                    for(int i=0; i<curApp.sourceFile.size();i++){
                        QUrl sourceFileUrl = QUrl::fromEncoded(curApp.sourceFile[i].toLocal8Bit());

                        if(sourceFileUrl == url && !jetiTransmitter.isEmpty()){
                            QString destPath;
                            #ifdef __APPLE__
                                // OSX path
                                destPath = jetiTransmitter[ui->jetiVolume->currentIndex()].rootPath + curApp.destinationPath[i] + "/";
                            #elif _WIN32
                                // WIN path
                                destPath = jetiTransmitter[ui->jetiVolume->currentIndex()].rootPath + curApp.destinationPath[i].right(curApp.destinationPath[i].size()-1) + "/";
                            #endif

                            qDebug() << "copy file to transmitter:" << destPath + url.fileName();

                            QFile file(destPath + url.fileName());
                            QDir dir;
                            if(!dir.exists(destPath))
                                dir.mkpath(destPath);

                            if (file.open(QFile::WriteOnly)) {
                                file.write(data->readAll());
                            }else{
                                qDebug() << "ERROR: Write file" << destPath + url.fileName();
                            }

                            break;
                        }
                    }
                }

            }
        }
    }
    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if(currentDownloads.isEmpty()){
        updateAppStatus();
    }
}

void MainWindow::loadSettings()
{
    qDebug()<<"read user settings";
    qDebug()<<"get app information...";

    QSettings settings;

    // hide debugLog
    if(settings.value("show_debuglog",DEFAULT_SHOW_DEBUGLOG).toBool()){
        QList<int> sizes;
        sizes << 200 << 10;
        ui->splitter_vertical->setSizes(sizes);
    }else{
        QList<int> sizes;
        sizes << 200 << 0;
        ui->splitter_vertical->setSizes(sizes);
    }

    // get sources
    getSourceList();
}

void MainWindow::debugLogPrint(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString formattedMessage = qFormatLogMessage(type, context, msg);
    ui->debugLogBrowser->append(formattedMessage);
}

void MainWindow::on_buttonInstall_clicked()
{

    QString currentApp = getCurrentAppName();

    if(luaApp.contains(currentApp)){
        ui->statusBar->showMessage("Installiere App: " + currentApp);
        qDebug() << "install app to transmitter:" << currentApp;

        appData app = luaApp.value(currentApp);

        // download sources
        QString source;
        foreach( source, app.sourceFile ){
            QUrl url = QUrl::fromEncoded(source.toLocal8Bit());
            appInfo appinfo;
            appinfo.appName = currentApp;
            appinfo.fileType = sourcefile;
            urlFile.insert(url.fileName(), appinfo);
            doDownload(url);
        }
    }
}

void MainWindow::on_buttonUninstall_clicked()
{
    QString currentApp = getCurrentAppName();

    if(luaApp.contains(currentApp)){
        ui->statusBar->showMessage("Deinstalliere App: " + currentApp);
        qDebug() << "uninstall app from transmitter:" << currentApp;

        appData app = luaApp.value(currentApp);

        for(int i=0;i<app.sourceFile.size();i++){
            QUrl url = QUrl::fromEncoded(app.sourceFile[i].toLocal8Bit());

            QString destPath;
            #ifdef __APPLE__
                // OSX path
                destPath = jetiTransmitter[ui->jetiVolume->currentIndex()].rootPath + app.destinationPath[i];
            #elif _WIN32
                // WIN path
                destPath = jetiTransmitter[ui->jetiVolume->currentIndex()].rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1);
            #endif

            QFile file(destPath+ "/" + url.fileName());
            QDir dir(destPath);
            // check if file exists and if yes: Is it really a file and no directory?
            if (file.exists()) {
                if(file.remove()){
                    qDebug() << "remove file from transmitter:" << file.fileName();
                }else{
                    qDebug() << "ERROR: failed to remove file:" << file.fileName();
                }
            }
            if(dir.isEmpty() && TRANSMITTER_APPFOLDER != app.destinationPath[i]){
                if(dir.rmdir(destPath)){
                    qDebug() << "remove folder from transmitter:" << dir.absolutePath();
                }else{
                    qDebug() << "ERROR: failed to remove folder:" << dir.absolutePath();
                }
            }
        }
    }

    updateAppStatus();
}

void MainWindow::on_actionPreferences_triggered()
{
    Preferences *preferences = new Preferences(this);

    connect(preferences, SIGNAL(updateSettings()),
            this, SLOT(loadSettings()));

    preferences->show();
}



