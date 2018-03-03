#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QDebug>

#include <QPainter>
#include <QAbstractItemDelegate>

#include "defaults.h"

class ListDelegate : public QAbstractItemDelegate
{

public:
    ListDelegate(QObject *parent = 0);
    virtual ~ListDelegate();

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    enum infoTyp{
        newApp,
        appUpdate,
        appInstalled
    };

};

#endif // LISTDELEGATE_H
