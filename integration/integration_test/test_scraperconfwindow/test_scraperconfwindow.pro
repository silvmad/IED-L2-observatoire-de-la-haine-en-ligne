QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \ 
    testscraperconfwindow.cpp \
    ../../integration_src/scraperconfwindow.cpp \
    ../../integration_src/conflist.cpp

HEADERS += \
    testscraperconfwindow.h \
    ../../integration_src/scraperconfwindow.h \
    ../../integration_src/conflist.h
