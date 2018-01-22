#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "defaults.h"

#include <QDialog>
#include <QDebug>
#include <QSettings>

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

signals:
    void updateSettings();

};

#endif // PREFERENCES_H
