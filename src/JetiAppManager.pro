#-------------------------------------------------
#
# Project created by QtCreator 2018-01-07T21:08:18
#
#-------------------------------------------------

QT       += core gui

QT       += network

QT       += webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JetiAppManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    previewpage.cpp \
    document.cpp \
    preferences.cpp \
    listdelegate.cpp

HEADERS  += mainwindow.h \
    previewpage.h \
    document.h \
    preferences.h \
    defaults.h \
    listdelegate.h

FORMS    += mainwindow.ui \
    preferences.ui

DISTFILES += \
    resources/marked.min.js \
    resources/qwebchannel.js \
    resources/markdown.css \
    resources/index.html \
    resources/default.md \
    jetiappmanager.ico

RESOURCES += \
    resources/markdowneditor.qrc


macx {
    # OS X: Specify icon resource to use
    ICON = jetiappmanager.ico

    # OS X: Specify our developer information using a custom plist
    QMAKE_INFO_PLIST = Info.plist
}

win32 {
    # Windows: Specify the icon to use
    RC_ICONS += jetiappmanager.ico
}
