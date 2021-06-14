#-------------------------------------------------
#
# Project created by QtCreator 2021-04-26T10:54:07
#
#-------------------------------------------------

QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = integration
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    scrapertab.cpp \
    analysertab.cpp \
    dbtab.cpp \
    analyserconfwindow.cpp \
    patternwindow.cpp \
    scraperconfwindow.cpp \
    keywordswindow.cpp \
    conflist.cpp \
    typeswindow.cpp

HEADERS += \
        mainwindow.h \
    scrapertab.h \
    analysertab.h \
    dbtab.h \
    analyserconfwindow.h \
    patternwindow.h \
    scraperconfwindow.h \
    keywordswindow.h \
    conflist.h \
    typeswindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
