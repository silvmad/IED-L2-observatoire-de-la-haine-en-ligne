QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

#QT += core sql testlib

#TEMPLATE = app
#CONFIG += console c++11
#CONFIG -= app_bundle testcase
#CONFIG -= qt

INCLUDEPATH += ../analyseur_src \
    ../../integration/integration_src

SOURCES += \
    testanalyseur.cpp \
    ../analyseur_src/func.cpp \
    ../../integration/integration_src/conflist.cpp

HEADERS += \
    testanalyseur.h
