#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);

    // read sources from settings
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QSettings settings;

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

    // read sources from textedit
    QString plainTextEditContents = ui->sources->toPlainText();
    QStringList sourceFiles = plainTextEditContents.split("\n");

    // save to settings
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QList<QString> data {sourceFiles};

    QSettings settings;
    settings.setValue("sourceList", QVariant::fromValue(data));

}
