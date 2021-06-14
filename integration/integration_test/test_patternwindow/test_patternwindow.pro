QT += testlib
QT += gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \ 
    testpatternwindow.cpp \
    ../../integration_src/conflist.cpp \
    ../../integration_src/analyserconfwindow.cpp \
    ../../integration_src/patternwindow.cpp \
    ../../integration_src/typeswindow.cpp

HEADERS += \
    testpatternwindow.h \
    ../../integration_src/analyserconfwindow.h \
    ../../integration_src/patternwindow.h \
    ../../integration_src/typeswindow.h
