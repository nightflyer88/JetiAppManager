#include "appinfo.h"
#include "ui_appinfo.h"

AppInfo::AppInfo(QWidget *parent, QStringList appList) :
    QDialog(parent),
    ui(new Ui::AppInfo)
{
    ui->setupUi(this);

    // Create model
    model = new QStringListModel(this);
    model->setStringList(appList);
    ui->listView->setModel(model);

    // set text label
    if(appList.count() > 1){
        ui->label->setText(QString::number(appList.count()) + tr(" Neue Apps sind verfügbar:"));
    }else{
        ui->label->setText(QString::number(appList.count()) + tr(" Neue App ist verfügbar:"));
    }

}

AppInfo::~AppInfo()
{
    delete ui;
}
