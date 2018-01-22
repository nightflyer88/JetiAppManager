#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);

    // read settings
    QSettings settings;

    ui->checkBox_debugLog->setChecked(settings.value("show_debuglog",DEFAULT_SHOW_DEBUGLOG).toBool());

    // read sources
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QList<QString> defUrl;
    defUrl << DEFAULT_SOURCE_URL;
    QList<QString> sourcelist = settings.value("sourceList",QVariant::fromValue(defUrl)).value<QList<QString>>();

    // print to textedit
    QString source;
    foreach( source, sourcelist ){
        ui->sources->appendPlainText(source);
    }

}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::accept()
{
    QDialog::accept();

    // save settings
    QSettings settings;

    settings.setValue("show_debuglog",ui->checkBox_debugLog->isChecked());

    // read sources from textedit
    QString plainTextEditContents = ui->sources->toPlainText();
    QStringList sourceFiles = plainTextEditContents.split("\n");

    // write to settings
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QList<QString> data {sourceFiles};
    settings.setValue("sourceList", QVariant::fromValue(data));

    emit(updateSettings());

}
