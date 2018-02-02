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
        this, SLOT(on_app_clicked()));

    // setup md-file viewer
    PreviewPage *page = new PreviewPage(this);
    ui->description->setPage(page);

    appDescription.setText(tr("## Jeti App Manager\n"
                              "- Sender am Computer anschliessen und Laufwerk wählen\n"
                              "- App aus der Liste auswählen\n"
                              "- Installieren drücken\n"
                              "- Fertig\n"
                              "\nViel Spass !"));

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
            if(!storage.isReadOnly() && isTransmitterValid(storage.rootPath()) && isTransmitterSupportLua(storage.rootPath())){
                transmitterData readDevice;
                readDevice.driveName = storage.displayName();
                readDevice.rootPath = storage.rootPath();
                readDevice = getDevice(readDevice);
                jetiTransmitter.append(readDevice);

                volumes.append(storage.displayName());
                qDebug("Jeti transmitter found: Type: %i Firmware: %.2f",readDevice.transmitterTyp, readDevice.firmwareVersion);
            }
        }
    }

    return volumes;
}

QStringList MainWindow::getAppSourceList(int hwTyp, MainWindow::appData app)
{
    QStringList sourcelist;

    if(isTransmitter14_16(hwTyp) && !app.sourceFile14_16.isEmpty())
        sourcelist = app.sourceFile14_16;

    if(isTransmitter24(hwTyp) && !app.sourceFile24.isEmpty())
        sourcelist = app.sourceFile24;

    if(!app.sourceFile.isEmpty())
        sourcelist = app.sourceFile;

    return sourcelist;
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

bool MainWindow::isTransmitterSupportLua(QString rootpath)
{
    QString destPath = TRANSMITTER_APPFOLDER;
    #ifdef __APPLE__
        // OSX path
        destPath = rootpath + destPath;
    #elif _WIN32
        // WIN path
        destPath = rootpath + destPath.right(destPath.size()-1);
    #endif

    QDir dir(destPath);
    // check if app folder exists, if true transmitter support LUA
    if (dir.exists()) {
        return true;
    }

    return false;
}

bool MainWindow::isTransmitterSupportApp(int hwTyp, QString appName)
{
    if(luaApp.contains(appName)){
        appData app = luaApp.value(appName);

        if(jetiTransmitter[getCurrentTransmitter()].firmwareVersion < app.requiredFirmware)
            return false;

        if(isTransmitter14_16(hwTyp) && !app.sourceFile14_16.isEmpty())
            return true;

        if(isTransmitter24(hwTyp) && !app.sourceFile24.isEmpty())
            return true;

        if(!app.sourceFile.isEmpty())
            return true;

    }

    return false;
}

bool MainWindow::isTransmitter14_16(int hwTyp)
{
    if(hwTyp==DC14 || hwTyp==DS14 || hwTyp==DC16 || hwTyp==DS16 )
        return true;

    return false;
}

bool MainWindow::isTransmitter24(int hwTyp)
{
    if(hwTyp==DC24 || hwTyp==DS24)
        return true;

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
                device.firmwareVersion = value["version"].toString().toFloat();

            if (value.contains("hw") && value["hw"].isDouble())
                device.transmitterTyp = value["hw"].toDouble();

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

        QStringList sourceList = getAppSourceList(jetiTransmitter[getCurrentTransmitter()].transmitterTyp, app);

        for(int i=0;i<sourceList.size();i++){
            QUrl url = QUrl::fromEncoded(sourceList[i].toLocal8Bit());

            QString destFile;
            #ifdef __APPLE__
                // OSX path
                destFile = jetiTransmitter[getCurrentTransmitter()].rootPath + app.destinationPath[i] + "/" + url.fileName();
            #elif _WIN32
                // WIN path
                destFile = jetiTransmitter[getCurrentTransmitter()].rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1) + "/" + url.fileName();
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
        item->setData(Qt::UserRole + 3, QString::number(app.requiredFirmware));
        item->setData(Qt::UserRole + 4, !app.sourceFile14_16.isEmpty() || !app.sourceFile.isEmpty());
        item->setData(Qt::UserRole + 5, !app.sourceFile24.isEmpty() || !app.sourceFile.isEmpty());
        item->setData(Qt::DecorationRole, app.previewImg);
        ui->appList->addItem(item);

    }

}

void MainWindow::getSourceList()
{
    ui->statusBar->showMessage(tr("Lade App Informationen..."));

    luaApp.clear();

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
        doDownload(url,"",sourceAppInfoFile);
    }
}

QString MainWindow::getCurrentAppName()
{
    QModelIndex index = ui->appList->currentIndex();
    return index.data(Qt::DisplayRole).toString();
}

int MainWindow::getCurrentTransmitter()
{
    return ui->jetiVolume->currentIndex();
}

void MainWindow::jetiVolume_changed(int index)
{

    if(index >= 0){
        QMetaEnum metaEnum = QMetaEnum::fromType<MainWindow::transmitterTyp>();
        ui->type->setText(metaEnum.valueToKey(jetiTransmitter[index].transmitterTyp));
        ui->firmware->setText(QString::number(jetiTransmitter[index].firmwareVersion));
    }else{
        ui->type->setText("-");
        ui->firmware->setText("-");
    }

    updateAppStatus();

}

void MainWindow::on_app_clicked()
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
            ui->statusBar->showMessage(tr("Lade Beschreibung..."));
            doDownload(url, itemText, descriptionfile);
        }else{
            appDescription.setText(tr("keine App Beschreibung verfügbar"));
        }

        updateAppStatus();
    }
}

void MainWindow::updateAppStatus()
{

    if(!jetiTransmitter.isEmpty() && isTransmitterSupportApp(jetiTransmitter[getCurrentTransmitter()].transmitterTyp, getCurrentAppName())){
        ui->buttonInstall->setHidden(false);

        if(isAppInstalled(getCurrentAppName())){
            ui->buttonInstall->setStyleSheet("background-color: rgb(15, 128, 255)");
            ui->buttonInstall->setText(tr("Aktualisieren"));
            ui->buttonUninstall->setStyleSheet("background-color: rgb(252, 1, 7)");
            ui->buttonUninstall->setHidden(false);
        }else{
            ui->buttonInstall->setStyleSheet("background-color: rgb(128, 255, 7)");
            ui->buttonInstall->setText(tr("Installieren"));
            ui->buttonUninstall->setHidden(true);
        }

    }else{
        ui->buttonInstall->setHidden(true);
        ui->buttonUninstall->setHidden(true);
    }

    ui->statusBar->clearMessage();
}

void MainWindow::doDownload(const QUrl &url, QString appName, int fileType)
{
    qDebug() << "download file:" << url.toString();

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    reply->setProperty("appName", QVariant(appName));
    reply->setProperty("fileType", QVariant(fileType));

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

            QString luaAppName = reply->property("appName").toString();
            int fileType = reply->property("fileType").toInt();

            if(fileType == sourceAppInfoFile){

                QString file = data->readAll();

                // encode source file
                QJsonDocument source = QJsonDocument::fromJson(file.toUtf8());
                if(source.isObject()){
                    QJsonObject appName = source.object();

                    foreach (QString name, appName.keys()) {
                        QJsonObject value = appName[name].toObject();
                        appData app;

                        // check if same app exist
                        if(luaApp.contains(name)){
                            int appCount = 0;
                            QString newName = name;
                            while(luaApp.contains(newName)){
                                appCount++;
                                newName = name + " [" +QString::number(appCount) +"]";
                            }
                            name = newName;
                        }

                        // Author name
                        if (value.contains("author") && value["author"].isString())
                            app.author = value["author"].toString();

                        // App version
                        if (value.contains("version") && value["version"].isString())
                            app.version = value["version"].toString();

                        // Preview image
                        if (value.contains("previewImg") && value["previewImg"].isString()){
                            QUrl urlImg = QUrl::fromEncoded(value["previewImg"].toString().toLocal8Bit());
                            if(urlImg.isValid()){
                                doDownload(urlImg,name, previewIcon);
                            }
                        }

                        // App description
                        if (value.contains("description") && value["description"].isString())
                            app.description = value["description"].toString();

                        // required transmitter firmware
                        if (value.contains("requiredFirmware") && value["requiredFirmware"].isDouble())
                            app.requiredFirmware = value["requiredFirmware"].toDouble();

                        // source files for all transmitters
                        if (value.contains("sourceFile") && value["sourceFile"].isArray()){
                            QJsonArray sourceFileArray = value["sourceFile"].toArray();

                            foreach (const QJsonValue& sourcefile, sourceFileArray) {
                                app.sourceFile.append(sourcefile.toString());
                            }
                        }

                        // source files only for DC/DS 14,16 transmitters
                        if (value.contains("sourceFile14_16") && value["sourceFile14_16"].isArray()){
                            QJsonArray sourceFileArray = value["sourceFile14_16"].toArray();

                            foreach (const QJsonValue& sourcefile, sourceFileArray) {
                                app.sourceFile14_16.append(sourcefile.toString());
                            }
                        }

                        // source files only for DC/DS 24 transmitter
                        if (value.contains("sourceFile24") && value["sourceFile24"].isArray()){
                            QJsonArray sourceFileArray = value["sourceFile24"].toArray();

                            foreach (const QJsonValue& sourcefile, sourceFileArray) {
                                app.sourceFile24.append(sourcefile.toString());
                            }
                        }

                        // destination path
                        if (value.contains("destinationPath") && value["destinationPath"].isArray()){
                            QJsonArray destinationPathArray = value["destinationPath"].toArray();

                            foreach (const QJsonValue& destinationpath, destinationPathArray) {
                                app.destinationPath.append(destinationpath.toString());
                            }
                        }

                        // add app to list
                        luaApp.insert(name, app);
                        updateAppList();
                    }

                }

            }else if(fileType == descriptionfile){

                // set app description
                QString file = data->readAll();
                appDescription.setText(file);

            }else if(fileType == previewIcon){

                // load preview icon
                if(luaApp.contains(luaAppName)){
                    appData curApp = luaApp.value(luaAppName);
                    curApp.previewImg.loadFromData(data->readAll());
                    luaApp[luaAppName] = curApp;

                    updateAppList();
                }

            }else if(fileType == sourcefile){

                // copy file to transmitter
                if(luaApp.contains(luaAppName)){

                    appData curApp = luaApp.value(luaAppName);
                    QStringList sourceList = getAppSourceList(jetiTransmitter[getCurrentTransmitter()].transmitterTyp, curApp);

                    for(int i=0; i < sourceList.size();i++){
                        QUrl sourceFileUrl = QUrl::fromEncoded(sourceList[i].toLocal8Bit());

                        if(sourceFileUrl == url && !jetiTransmitter.isEmpty()){
                            QString destPath;
                            #ifdef __APPLE__
                                // OSX path
                                destPath = jetiTransmitter[getCurrentTransmitter()].rootPath + curApp.destinationPath[i] + "/";
                            #elif _WIN32
                                // WIN path
                                destPath = jetiTransmitter[getCurrentTransmitter()].rootPath + curApp.destinationPath[i].right(curApp.destinationPath[i].size()-1) + "/";
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

    if(luaApp.contains(currentApp) && isTransmitterSupportApp(jetiTransmitter[getCurrentTransmitter()].transmitterTyp, currentApp)){
        ui->statusBar->showMessage(tr("Installiere App: ") + currentApp);
        qDebug() << "install app to transmitter:" << currentApp;

        appData app = luaApp.value(currentApp);

        // download sources
        foreach(QString source, getAppSourceList(jetiTransmitter[getCurrentTransmitter()].transmitterTyp, app)){
            QUrl url = QUrl::fromEncoded(source.toLocal8Bit());
            doDownload(url, currentApp, sourcefile);
        }
    }
}

void MainWindow::on_buttonUninstall_clicked()
{
    QString currentApp = getCurrentAppName();

    if(luaApp.contains(currentApp)){
        ui->statusBar->showMessage(tr("Deinstalliere App: ") + currentApp);
        qDebug() << "uninstall app from transmitter:" << currentApp;

        appData app = luaApp.value(currentApp);
        QStringList sourceList = getAppSourceList(jetiTransmitter[getCurrentTransmitter()].transmitterTyp, app);

        for(int i=0;i<sourceList.size();i++){
            QUrl url = QUrl::fromEncoded(sourceList[i].toLocal8Bit());

            QString destPath;
            #ifdef __APPLE__
                // OSX path
                destPath = jetiTransmitter[getCurrentTransmitter()].rootPath + app.destinationPath[i];
            #elif _WIN32
                // WIN path
                destPath = jetiTransmitter[getCurrentTransmitter()].rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1);
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



