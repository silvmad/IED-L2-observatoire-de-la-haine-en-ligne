QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \ 
    testconflist.cpp \
    ../../integration_src/conflist.cpp

HEADERS += \
    testconflist.h
