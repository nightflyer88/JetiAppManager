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
    ui->checkBox_show_newApps->setChecked(settings.value("show_newApps",DEFAULT_SHOW_NEWAPPS).toBool());
    ui->checkBox_show_appUpdates->setChecked(settings.value("show_appUpdates",DEFAULT_SHOW_APPUPDATES).toBool());

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

    ui->setLanguage->addItems(listAvailableLanguages());
    ui->setLanguage->setCurrentIndex(getSavedLanguageIndex());

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
    settings.setValue("show_newApps",ui->checkBox_show_newApps->isChecked());
    settings.setValue("show_appUpdates",ui->checkBox_show_appUpdates->isChecked());

    // read sources from textedit
    QString plainTextEditContents = ui->sources->toPlainText();
    QStringList sourceFiles = plainTextEditContents.split("\n");

    // write to settings
    qRegisterMetaTypeStreamOperators<QList<QString>>("Data");
    QList<QString> data {sourceFiles};
    settings.setValue("sourceList", QVariant::fromValue(data));

    // save language
    settings.setValue("language", getSelectetLanguageFile());

    emit(updateSettings());

}

QStringList Preferences::listAvailableLanguages()
{
    QDir directory(":/language/");

    QStringList qmfilter;
    qmfilter << "*.qm";
    QStringList filename = directory.entryList(qmfilter);

    QStringList languagesList;
    languagesList << QLocale::languageToString(QLocale(DEFAULT_LANGUAGE).language());

    for (int i = 0; i < filename.size(); ++i){
        // get locale extracted by filename
        QString locale;
        locale = filename[i]; // "JetiAppManager_en.qm"
        locale.truncate(locale.lastIndexOf('.')); // "JetiAppManager_en"
        locale.remove(0, QString::fromUtf8("JetiAppManager_").length()); // "en"
        languagesList << QLocale::languageToString(QLocale(locale).language());

    }

    return languagesList;
}

QStringList Preferences::listAvailableLanguagesFiles()
{
    QDir directory(":/language/");

    QStringList qmfilter;
    qmfilter << "*.qm";
    QStringList filename = directory.entryList(qmfilter);

    QStringList languagesList;
    languagesList << QLocale::languageToString(QLocale(DEFAULT_LANGUAGE).language());

    for (int i = 0; i < filename.size(); ++i){
        languagesList << filename[i]; // "JetiAppManager_en.qm"
    }

    return languagesList;
}

QString Preferences::getSelectetLanguageFile()
{
    QString selectedLanguage;

    QStringList languagesList;
    languagesList = listAvailableLanguagesFiles();

    selectedLanguage = languagesList[ui->setLanguage->currentIndex()];

    return selectedLanguage;
}

int Preferences::getSavedLanguageIndex()
{
    QSettings settings;

    QString languageFile = settings.value("language", DEFAULT_LANGUAGE).toString();

    QStringList languagesList;
    languagesList = listAvailableLanguagesFiles();

    int index = 0;

    for (int i = 0; i < languagesList.size(); ++i){
        if(languagesList[i] == languageFile){
            index = i;
            break;
        }
    }

    return index;
}
