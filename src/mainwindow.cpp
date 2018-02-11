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

    qDebug() << "### Jeti App Manager" << JETIAPPMANAGER_VERSION << "(C) 2018 M.Lehmann ###";


    // init buttons
    ui->buttonUninstall->setHidden(true);
    ui->buttonInstall->setHidden(true);


    // init interval timer for searching volumes
    QTimer *searchVolume = new QTimer(this);
    connect(searchVolume, SIGNAL(timeout()), this, SLOT(checkMountedVolumes()));
    searchVolume->start(1000);
    connect(ui->jetiVolume, SIGNAL(currentIndexChanged(int)), this, SLOT(jetiVolume_changed(int)));


    // connect signals for app list
    connect(ui->appList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this, SLOT(on_app_clicked()));
    connect(appManager, SIGNAL(hasNewApp()),this, SLOT(reloadAppList()));

    // connect signals
    connect(appManager, SIGNAL(hasNewAppStatus()),this, SLOT(reloadAppStatus()));
    connect(appManager, SIGNAL(hasNewAppDescription(QString)),this, SLOT(reloadAppDescription(QString)));


    // setup md-file viewer
    PreviewPage *page = new PreviewPage(this);
    ui->description->setPage(page);

    appDescription.setText("## Jeti App Manager "
                           JETIAPPMANAGER_VERSION
                           "\n(C) 2018 M.Lehmann\n"+
                           tr("- Sender am Computer anschliessen und Laufwerk wählen\n"
                              "- App aus der Liste auswählen\n"
                              "- Installieren drücken\n"
                              "- Fertig"));

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &appDescription);
    page->setWebChannel(channel);

    ui->description->setUrl(QUrl("qrc:/index.html"));


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
        appManager->transmitterList.clear();
        ui->jetiVolume->clear();
        ui->jetiVolume->addItems(appManager->transmitterVolumes());
    }

}

void MainWindow::reloadAppList(QRegExp regExp)
{
    // print apps in list
    ui->appList->clear();

    QStringList list = appManager->applist.keys();

    for(int i = 0; i < list.size(); i++){
        AppManager::App app = appManager->applist.value(list[i]);
        QString newitem = list[i] + "*@*" + app.author;
        list.replaceInStrings(list[i],newitem);
    }

    list = list.filter(regExp);
    std::sort(list.begin(), list.end());

    foreach(QString item, list){
        QStringList newlist = item.split("*@*");
        list.replaceInStrings(item,newlist[0]);
    }

    ui->appList->setItemDelegate(new ListDelegate(ui->appList));

    for(int i = 0; i < list.size(); i++){
        AppManager::App app = appManager->applist.value(list[i]);

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, list[i]);
        item->setData(Qt::UserRole + 1, tr("Version: ") + app.version);
        item->setData(Qt::UserRole + 2, tr("Author: ") + app.author);
        item->setData(Qt::UserRole + 3, QString::number(app.requiredFirmware));
        item->setData(Qt::UserRole + 4, !app.sourceFile14_16.isEmpty() || !app.sourceFile.isEmpty());
        item->setData(Qt::UserRole + 5, !app.sourceFile24.isEmpty() || !app.sourceFile.isEmpty());
        item->setData(Qt::DecorationRole, app.previewImg);
        ui->appList->addItem(item);

    }

    ui->appCount->setText(QString::number(list.size()));

}

void MainWindow::reloadAppDescription(QString file)
{
    appDescription.setText(file);

    reloadAppStatus();
}

void MainWindow::getAppInformation()
{
    ui->statusBar->showMessage(tr("Lade App Informationen..."));

    // read sources from settings
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QSettings settings;

    QList<QString> defUrl;
    defUrl << DEFAULT_SOURCE_URL;
    QList<QString> sourcelist = settings.value("sourceList",QVariant::fromValue(defUrl)).value<QList<QString>>();

    appManager->downloadAppInformation(sourcelist);
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
        appManager->currentTransmitterIndex = index;
        ui->type->setText(appManager->getTransmitterName(appManager->transmitterList[index]));
        ui->firmware->setText(QString::number(appManager->transmitterList[index].firmwareVersion));
    }else{
        ui->type->setText("-");
        ui->firmware->setText("-");
    }

    reloadAppStatus();

}

void MainWindow::on_app_clicked()
{
    // show app description
    ui->statusBar->showMessage(tr("Lade Beschreibung..."));

    appManager->getAppDescription(getCurrentAppName());
}

void MainWindow::reloadAppStatus()
{

    if(!appManager->transmitterList.isEmpty() && appManager->isTransmitterSupportApp(appManager->transmitterList[getCurrentTransmitter()], getCurrentAppName())){
        ui->buttonInstall->setHidden(false);

        if(appManager->isAppInstalled(appManager->transmitterList[getCurrentTransmitter()],getCurrentAppName())){
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

void MainWindow::loadSettings()
{
    QSettings settings;

    qDebug()<<"read user settings from file:" << settings.fileName();

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

    getAppInformation();
}

void MainWindow::debugLogPrint(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString formattedMessage = qFormatLogMessage(type, context, msg);
    ui->debugLogBrowser->append(formattedMessage);
}

void MainWindow::on_buttonInstall_clicked()
{

    QString currentApp = getCurrentAppName();

    ui->statusBar->showMessage(tr("Installiere App: ") + currentApp);

    appManager->installApp(appManager->transmitterList[getCurrentTransmitter()], currentApp);

}

void MainWindow::on_buttonUninstall_clicked()
{
    QString currentApp = getCurrentAppName();

    ui->statusBar->showMessage(tr("Deinstalliere App: ") + currentApp);

    if(appManager->uninstallApp(appManager->transmitterList[getCurrentTransmitter()], currentApp)){
        QMessageBox messageBox;
        messageBox.critical(0,"Error",currentApp + tr(" konnte nicht deinstalliert werden !"));
    }

    reloadAppStatus();

}

void MainWindow::on_actionPreferences_triggered()
{
    Preferences *preferences = new Preferences(this);

    connect(preferences, SIGNAL(updateSettings()),
            this, SLOT(loadSettings()));

    preferences->show();
}


void MainWindow::on_searchText_textChanged(const QString &arg1)
{
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);

    reloadAppList(regExp);

}

void MainWindow::on_actionHelp_triggered()
{
    QUrl url = QUrl::fromEncoded(HELP_FILE_URL);
    ui->statusBar->showMessage(tr("Lade Hilfe..."));
    appManager->doDownload(url, "help", appManager->descriptionfile);
}
