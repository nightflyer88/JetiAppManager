#include "listdelegate.h"

ListDelegate::ListDelegate(QObject *parent)
{

}

ListDelegate::~ListDelegate()
{

}

void ListDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);


    QPen fontColor;


    if(option.state & QStyle::State_Selected){
        QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        //BORDER
        painter->setPen(lineMarkedPen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        //painter->setPen(fontMarkedPen);
        fontColor = fontMarkedPen;

    } else {
        //BACKGROUND
                    //ALTERNATING COLORS
        painter->setBrush( (index.row() % 2) ? Qt::white : QColor(252,252,252) );
        painter->drawRect(r);

        //BORDER
        painter->setPen(linePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        //painter->setPen(fontPen);
        fontColor = fontPen;
    }

    //Get app data
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString appName = index.data(Qt::DisplayRole).toString();
    QString appVersion = index.data(Qt::UserRole + 1).toString();
    QString appAuthor = index.data(Qt::UserRole + 2).toString();
    QString requiredTransmitterFirmware = "V" + index.data(Qt::UserRole + 3).toString();
    bool support14_16 = index.data(Qt::UserRole + 4).toBool();
    bool support24 = index.data(Qt::UserRole + 5).toBool();
    QString appFlagText = index.data(Qt::UserRole + 6).toString();
    int appStatus = index.data(Qt::UserRole + 7).toInt();


    // ICON
    int imageSpace = 100;
    if (!ic.isNull()) {
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r.left(), r.top(), 80, 80, Qt::AlignVCenter|Qt::AlignLeft);
    }

    // New App flag
    if(appFlagText != ""){
        r = option.rect.adjusted(0, 0, -50, -50);
        QPainterPath path;
        path.moveTo(r.left() + 20, r.top());
        path.lineTo(r.left(),r.top() + 20);
        path.lineTo(r.left(),r.top() + 50);
        path.lineTo(r.left() + 50,r.top());
        path.lineTo(r.left() + 20,r.top());
        if(appStatus == newApp){
            painter->fillPath(path, QBrush(QColor::fromRgb(255,38,0)));
        }else if(appStatus == appUpdate){
            painter->fillPath(path, QBrush(QColor::fromRgb(0,127,255)));
        }else if(appStatus == appInstalled){
            painter->fillPath(path, QBrush(QColor::fromRgb(128,222,6)));
        }


        painter->save();
        painter->translate(r.left(),r.top());
        painter->rotate(-45);
        painter->setPen(fontMarkedPen);
        if(appStatus == newApp){
            painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_NEW, QFont::Bold ) );
        }else if(appStatus == appUpdate){
            painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_UPDATE, QFont::Bold ) );
        }else if(appStatus == appInstalled){
            painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_INSTALLED, QFont::Bold ) );
        }
        painter->drawText(-25, 0, 50, 50, Qt::AlignCenter|Qt::AlignHCenter, appFlagText, &r);
        painter->rotate(45);
        painter->restore();
    }

    int tagSpace = imageSpace;

    // support all transmitters
    if(support14_16 && support24){
        r = option.rect.adjusted(tagSpace, 10, -10, -50);
        QPainterPath path;
        path.addRoundedRect(r.left(), r.top(), 60, 16, 8, 8);
        painter->fillPath(path, QColor::fromRgb(128,222,6));
        painter->setPen(fontMarkedPen);
        painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_TAG, QFont::Bold ) );
        painter->drawText(r.left(), r.top(), 60, 16, Qt::AlignCenter|Qt::AlignHCenter, "all Tx", &r);
        tagSpace += 70;
    }else{
        // support 14 transmitters
        if(support14_16){
            r = option.rect.adjusted(tagSpace, 10, -10, -50);
            QPainterPath path;
            path.addRoundedRect(r.left(), r.top(), 60, 16, 8, 8);
            painter->fillPath(path, QColor::fromRgb(255,147,0));
            painter->setPen(fontMarkedPen);
            painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_TAG, QFont::Bold ) );
            painter->drawText(r.left(), r.top(), 60, 16, Qt::AlignCenter|Qt::AlignHCenter, "DC/DS14", &r);
            tagSpace += 70;
        }

        // support 16 transmitters
        if(support14_16){
            r = option.rect.adjusted(tagSpace, 10, -10, -50);
            QPainterPath path;
            path.addRoundedRect(r.left(), r.top(), 60, 16, 8, 8);
            painter->fillPath(path, QColor::fromRgb(255,38,0));
            painter->setPen(fontMarkedPen);
            painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_TAG, QFont::Bold ) );
            painter->drawText(r.left(), r.top(), 60, 16, Qt::AlignCenter|Qt::AlignHCenter, "DC/DS16", &r);
            tagSpace += 70;
        }

        // support 24 transmitters
        if(support24){
            r = option.rect.adjusted(tagSpace, 10, -10, -50);
            QPainterPath path;
            path.addRoundedRect(r.left(), r.top(), 60, 16, 8, 8);
            painter->fillPath(path, QColor::fromRgb(148,23,81));
            painter->setPen(fontMarkedPen);
            painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_TAG, QFont::Bold ) );
            painter->drawText(r.left(), r.top(), 60, 16, Qt::AlignCenter|Qt::AlignHCenter, "DC/DS24", &r);
            tagSpace += 70;
        }
    }

    // Firmware tag
    if(requiredTransmitterFirmware != "V0"){
        r = option.rect.adjusted(tagSpace, 10, -10, -50);
        QPainterPath path;
        path.addRoundedRect(r.left(), r.top(), 50, 16, 8, 8);
        painter->fillPath(path, QColor::fromRgb(0,150,255));
        painter->setPen(fontMarkedPen);
        painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_TAG, QFont::Bold ) );
        painter->drawText(r.left(), r.top(), 50, 16, Qt::AlignCenter|Qt::AlignHCenter, requiredTransmitterFirmware, &r);
    }




    painter->setPen(fontColor);

    //app name
    r = option.rect.adjusted(imageSpace, 0, -10, -47);
    painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_DESCRIPTION, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, appName, &r);

    //version
    r = option.rect.adjusted(imageSpace, 0, -10, -30);
    painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_VERSION, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, appVersion, &r);

    //author
    r = option.rect.adjusted(imageSpace, 0, -10, -15);
    painter->setFont( QFont( "Lucida Grande", FONTSIZE_APP_AUTHOR, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, appAuthor, &r);
}

QSize ListDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    return QSize(200, 100); // very dumb value
}
