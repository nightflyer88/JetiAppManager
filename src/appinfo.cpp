#include "appinfo.h"
#include "ui_appinfo.h"

AppInfo::AppInfo(QWidget *parent, QStringList appList, int infoTyp) :
    QDialog(parent),
    ui(new Ui::AppInfo)
{
    ui->setupUi(this);

    // Create model
    model = new QStringListModel(this);
    model->setStringList(appList);
    ui->listView->setModel(model);

    // set text label
    if(infoTyp == newAppsAvailable){
        this->setWindowTitle(tr("Neue Apps"));
        if(appList.count() > 1){
            ui->label->setText(QString::number(appList.count()) + tr(" Neue Apps sind verfügbar:"));
        }else{
            ui->label->setText(QString::number(appList.count()) + tr(" Neue App ist verfügbar:"));
        }
    }else if(infoTyp == appUpdatesAvailable){
        this->setWindowTitle(tr("App Updates"));
        if(appList.count() > 1){
            ui->label->setText(QString::number(appList.count()) + tr(" App Updates sind verfügbar:"));
        }else{
            ui->label->setText(QString::number(appList.count()) + tr(" App Update ist verfügbar:"));
        }
    }

}

AppInfo::~AppInfo()
{
    delete ui;
}
