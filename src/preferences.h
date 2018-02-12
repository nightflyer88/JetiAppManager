#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "defaults.h"

#include <QDebug>
#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QFile>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = 0);
    ~Preferences();

private:
    Ui::Preferences *ui;

    void accept();

    static QStringList listAvailableLanguages();

    static QStringList listAvailableLanguagesFiles();

    QString getSelectetLanguageFile();

    int getSavedLanguageIndex();

    QStringList languageFile;

signals:
    void updateSettings();

};

#endif // PREFERENCES_H
