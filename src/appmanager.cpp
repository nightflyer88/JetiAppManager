#include "appmanager.h"

AppManager::AppManager(QObject *parent) : QObject(parent)
{
    // setup downloadmanager
    connect(&manager, SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));

}

void AppManager::close()
{

}

void AppManager::downloadAppInformation(QList<QString> sourcelist)
{
    qDebug()<<"download app information:";

    applist.clear();

    QString source;
    foreach( source, sourcelist ){
        QUrl url = QUrl::fromEncoded(source.toLocal8Bit());
        doDownload(url,"",appInfofile);
    }
}

void AppManager::getAppDescription(QString appName)
{
    if(applist.contains(appName)){
        App app;
        app = applist.value(appName);

        // show description
        QUrl url = QUrl::fromEncoded(app.description.toLocal8Bit());
        if(url.isValid()){
            doDownload(url, appName, descriptionfile);
        }else{
            emit(hasNewAppDescription(tr("keine App Beschreibung verfügbar")));
        }
    }
}

void AppManager::installApp(Transmitter transmitter, QString appName)
{
    error = false;

    if(applist.contains(appName) && isTransmitterSupportApp(transmitter, appName)){
        qDebug() << "install app to transmitter:" << appName;

        App app = applist.value(appName);
        app.doInstall = true;
        applist[appName] = app;

        // download sources
        foreach(QString source, getAppSourceList(transmitter, app)){
            QUrl url = QUrl::fromEncoded(source.toLocal8Bit());
            doDownload(url, appName, sourcefile);
        }
    }
}

bool AppManager::uninstallApp(Transmitter transmitter, QString appName)
{
    if(applist.contains(appName)){
        qDebug() << "uninstall app from transmitter:" << appName;

        App app = applist.value(appName);
        app.doUninstall = true;
        error = false;

        QStringList sourceList = getAppSourceList(transmitter, app);

        for(int i=0;i<sourceList.size();i++){
            QUrl url = QUrl::fromEncoded(sourceList[i].toLocal8Bit());

            QString destPath;
            QString appFolderPath = TRANSMITTER_APPFOLDER;
            #ifdef __APPLE__
                // OSX path
                destPath = transmitter.rootPath + app.destinationPath[i];
                appFolderPath = transmitter.rootPath + appFolderPath;
            #elif _WIN32
                // WIN path
                destPath = transmitter.rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1);
                appFolderPath = transmitter.rootPath + appFolderPath.right(appFolderPath.size()-1);
            #endif

            QFile file(destPath+ "/" + url.fileName());
            QDir dir(destPath);
            QDir appDir(appFolderPath);
            // check if file exists and if yes: Is it really a file and no directory?
            if (file.exists()) {
                if(file.remove()){
                    qDebug() << "remove file from transmitter:" << file.fileName();
                }else{
                    qDebug() << "ERROR: failed to remove file:" << file.fileName();
                    error = true;
                }
            }

            while(dir.isEmpty() && (dir != appDir)){
                if(dir.rmdir(dir.absolutePath())){
                    qDebug() << "remove folder from transmitter:" << dir.absolutePath();
                }else{
                    qDebug() << "ERROR: failed to remove folder:" << dir.absolutePath();
                    error = true;
                    break;
                }
                dir.cdUp();
            }


        }

        if(error){
            qDebug() << "ERROR: App uninstall failed";
        }else{
            qDebug() << "app successfully uninstalled";
        }

        app.doUninstall = false;

        return error;
    }
}

void AppManager::doDownload(const QUrl &url, QString appName, int fileType)
{
    qDebug() << "download file:" << url.toString();

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    reply->setProperty("appName", QVariant(appName));
    reply->setProperty("fileType", QVariant(fileType));

    currentDownloads.append(reply);
}

bool AppManager::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

bool AppManager::isTransmitterValid(QString rootpath)
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

bool AppManager::isTransmitterSupportLua(QString rootpath)
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

void AppManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    QString appName = reply->property("appName").toString();
    int fileType = reply->property("fileType").toInt();

    if (reply->error()) {
        qDebug("ERROR: Download of %s failed: %s",url.toEncoded().constData(),qPrintable(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            qDebug("ERROR: Request was redirected");
        } else {
            QIODevice *data = reply;

            if(fileType == appInfofile){

                QString file = data->readAll();

                // encode source file
                QJsonDocument source = QJsonDocument::fromJson(file.toUtf8());
                if(source.isObject()){
                    QJsonObject appName = source.object();

                    foreach (QString name, appName.keys()) {
                        QJsonObject value = appName[name].toObject();
                        App app;

                        // check if same app exist
                        if(applist.contains(name)){
                            int appCount = 0;
                            QString newName = name;
                            while(applist.contains(newName)){
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
                        applist.insert(name, app);
                        emit(hasNewApp());
                    }

                }else{
                    qDebug("ERROR: json syntax error in file: %s",url.toEncoded().constData());
                }

            }else if(fileType == descriptionfile){

                // app description
                QString file = data->readAll();
                emit(hasNewAppDescription(file));

            }else if(fileType == previewIcon){

                // load preview icon
                if(applist.contains(appName)){
                    App curApp = applist.value(appName);
                    curApp.previewImg.loadFromData(data->readAll());
                    applist[appName] = curApp;

                    emit(hasNewApp());
                }

            }else if(fileType == sourcefile){

                // copy file to transmitter
                if(applist.contains(appName)){

                    App app = applist.value(appName);
                    QStringList sourceList = getAppSourceList(transmitterList[currentTransmitterIndex], app);

                    for(int i=0; i < sourceList.size();i++){
                        QUrl sourceFileUrl = QUrl::fromEncoded(sourceList[i].toLocal8Bit());

                        if(sourceFileUrl == url && !transmitterList.isEmpty()){
                            QString destPath;
                            #ifdef __APPLE__
                                // OSX path
                                destPath = transmitterList[currentTransmitterIndex].rootPath + app.destinationPath[i] + "/";
                            #elif _WIN32
                                // WIN path
                                destPath = transmitterList[currentTransmitterIndex].rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1) + "/";
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
                                error = true;
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
        emit(hasNewAppStatus());

        if(applist.contains(appName)){
            App app = applist.value(appName);
            if(app.doInstall){
                if(error){
                    qDebug() << "ERROR: App install failed";
                }else{
                    qDebug() << "app successfully installed";
                }

                app.doInstall = false;
                applist[appName] = app;

                error = false;
            }
        }
    }
}

bool AppManager::isTransmitterSupportApp(Transmitter transmitter, QString appName)
{
    if(applist.contains(appName)){
        App app = applist.value(appName);

        if(transmitter.firmwareVersion < app.requiredFirmware)
            return false;

        if(isTransmitter14_16(transmitter.device) && !app.sourceFile14_16.isEmpty())
            return true;

        if(isTransmitter24(transmitter.device) && !app.sourceFile24.isEmpty())
            return true;

        if(!app.sourceFile.isEmpty())
            return true;

    }

    return false;
}

bool AppManager::isTransmitter14_16(int hwTyp)
{
    if(hwTyp==DC_14 || hwTyp==DS_14 || hwTyp==DC_16 || hwTyp==DS_16 )
        return true;

    return false;
}

bool AppManager::isTransmitter24(int hwTyp)
{
    if(hwTyp==DC_24 || hwTyp==DS_24)
        return true;

    return false;
}

bool AppManager::isAppInstalled(Transmitter transmitter, QString appName)
{
    if(applist.contains(appName)){
        App app = applist.value(appName);

        QStringList sourceList = getAppSourceList(transmitter, app);

        for(int i=0;i<sourceList.size();i++){
            QUrl url = QUrl::fromEncoded(sourceList[i].toLocal8Bit());

            QString destFile;
            #ifdef __APPLE__
                // OSX path
                destFile = transmitter.rootPath + app.destinationPath[i] + "/" + url.fileName();
            #elif _WIN32
                // WIN path
                destFile = transmitter.rootPath + app.destinationPath[i].right(app.destinationPath[i].size()-1) + "/" + url.fileName();
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

AppManager::Transmitter AppManager::getTransmitterDevice(Transmitter transmitter)
{
    QFile configfile(transmitter.rootPath + TRANSMITTER_CONFIGFILE);

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
                transmitter.firmwareVersion = value["version"].toString().toFloat();

            if (value.contains("hw") && value["hw"].isDouble())
                transmitter.device = value["hw"].toDouble();

        }

    }else{
        qDebug() << "ERROR: no config file on transmitter found !";
    }

    return transmitter;
}

QString AppManager::getTransmitterName(Transmitter transmitter)
{
    if(transmitter.device==DS_24)
        return "DS24";

    if(transmitter.device==DC_24)
        return "DC24";

    if(transmitter.device==DS_16)
        return "DS16";

    if(transmitter.device==DC_16)
        return "DC16";

    if(transmitter.device==DS_14)
        return "DS14";

    if(transmitter.device==DC_14)
        return "DC14";

    return "-";
}

QStringList AppManager::transmitterVolumes()
{
    QStringList volumes;

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if(!storage.isReadOnly() && isTransmitterValid(storage.rootPath()) && isTransmitterSupportLua(storage.rootPath())){
                Transmitter transmitter;
                transmitter.driveName = storage.displayName();
                transmitter.rootPath = storage.rootPath();
                transmitter = getTransmitterDevice(transmitter);
                transmitterList.append(transmitter);

                volumes.append(storage.displayName());
                qDebug("Jeti transmitter found: Type: %i Firmware: %.2f", transmitter.device, transmitter.firmwareVersion);
            }
        }
    }

    return volumes;
}

QStringList AppManager::getAppSourceList(Transmitter transmitter, App app)
{
    QStringList sourcelist;

    if(isTransmitter14_16(transmitter.device) && !app.sourceFile14_16.isEmpty())
        sourcelist = app.sourceFile14_16;

    if(isTransmitter24(transmitter.device) && !app.sourceFile24.isEmpty())
        sourcelist = app.sourceFile24;

    if(!app.sourceFile.isEmpty())
        sourcelist = app.sourceFile;

    return sourcelist;
}


