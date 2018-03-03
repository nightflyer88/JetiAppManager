#ifndef APPINFO_H
#define APPINFO_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

namespace Ui {
class AppInfo;
}

class AppInfo : public QDialog
{
    Q_OBJECT

public:
    explicit AppInfo(QWidget *parent = 0, QStringList appList = {}, int infoTyp = newAppsAvailable);
    ~AppInfo();

    enum infoTyp{
        newAppsAvailable,
        appUpdatesAvailable
    };

private:
    Ui::AppInfo *ui;

    QStringListModel *model;
};

#endif // APPINFO_H
