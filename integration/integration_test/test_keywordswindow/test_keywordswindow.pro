QT += testlib sql
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \  
    testkeywordswindow.cpp \
    ../../integration_src/keywordswindow.cpp

HEADERS += \
    testkeywordswindow.h \
    ../../integration_src/keywordswindow.h
