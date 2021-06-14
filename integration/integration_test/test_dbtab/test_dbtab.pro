QT += testlib sql
QT += gui
CONFIG += qt warn_on depend_includepath testcase
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \ 
    testdbtab.cpp \
    ../../integration_src/scrapertab.cpp \
    ../../integration_src/scraperconfwindow.cpp \
    ../../integration_src/keywordswindow.cpp \
    ../../integration_src/conflist.cpp \
    ../../integration_src/mainwindow.cpp \
    ../../integration_src/analysertab.cpp \
    ../../integration_src/analyserconfwindow.cpp \
    ../../integration_src/dbtab.cpp \
    ../../integration_src/patternwindow.cpp \
    ../../integration_src/typeswindow.cpp

HEADERS += \
    testdbtab.h \
    ../../integration_src/scrapertab.h \
    ../../integration_src/scraperconfwindow.h \
    ../../integration_src/keywordswindow.h \
    ../../integration_src/conflist.h \
    ../../integration_src/mainwindow.h \
    ../../integration_src/analysertab.h \
    ../../integration_src/analyserconfwindow.h \
    ../../integration_src/dbtab.h \
    ../../integration_src/patternwindow.h \
    ../../integration_src/typeswindow.h
