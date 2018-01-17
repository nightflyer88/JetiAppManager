#include <QCoreApplication>

#include "mainwindow.h"
#include "previewpage.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup app list
    connect(ui->appList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
        this, SLOT(itemPressed()));

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

    // setup jeti volumes
    ui->jetiVolume->addItems(getVolumes());

    // get sources
    getSourceList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPreferences_triggered()
{
    Preferences *preferences = new Preferences(this);

    preferences->show();
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

QStringList MainWindow::getVolumes()
{
    QStringList volumes;

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!storage.isReadOnly()) {

                QString destPath;
                #ifdef __APPLE__
                    // OSX path
                    destPath = "/Volumes/" + storage.displayName() + "/Apps";
                #elif _WIN32
                    // WIN path
                    destPath = storage.displayName() + "/Apps";
                #endif

                // check if Apps folder available on volume
                QDir dir;
                if(dir.exists(destPath)){
                    volumes.append(storage.displayName());
                }

            }
        }
    }

    return volumes;
}

void MainWindow::on_buttonInstall_clicked()
{
    appData app;

    if(luaApp.contains(currentApp)){
        app = luaApp.value(currentApp);

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

void MainWindow::itemPressed()
{
    QModelIndex index = ui->appList->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();

    appData app;

    if(luaApp.contains(itemText)){
        app = luaApp.value(itemText);
        currentApp = itemText;

        // show description
        QUrl url = QUrl::fromEncoded(app.description.toLocal8Bit());
        if(url.isValid()){
            appInfo appinfo;
            appinfo.fileType = descriptionfile;
            urlFile.insert(url.fileName(), appinfo);
            doDownload(url);
        }else{
            appDescription.setText("keine App Beschreibung verfügbar");
        }
    }else{
        currentApp = "";
    }

}

void MainWindow::doDownload(const QUrl &url)
{
    qDebug() << "download:" << url.toString();
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
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            fputs("Request was redirected.\n", stderr);
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

                            //if (value.contains("previewImg") && value["previewImg"].isString())
                            //    app.previewImgUrl = value["previewImg"].toString();

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

                        if(sourceFileUrl == url){
                            QString destPath;
                            #ifdef __APPLE__
                                // OSX path
                                destPath = "/Volumes/" + ui->jetiVolume->currentText() + curApp.destinationPath[i] + "/";
                            #elif _WIN32
                                // WIN path
                                destPath = ui->jetiVolume->currentText() + curApp.destinationPath[i].right(curApp.destinationPath[i].size()-1) + "/";
                            #endif

                            qDebug() << "copy file:" << destPath + url.fileName();

                            QFile file(destPath + url.fileName());
                            QDir dir;
                            if(!dir.exists(destPath))
                                dir.mkpath(destPath);

                            if (file.open(QFile::WriteOnly)) {
                                file.write(data->readAll());
                            }else{
                                qDebug() << "ERROR write file:" << destPath + url.fileName();
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
}

