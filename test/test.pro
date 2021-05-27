QT += core sql testlib

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle testcase
#CONFIG -= qt

INCLUDEPATH += ../analyseur

SOURCES += \
    testanalyseur.cpp \
    ../analyseur/func.cpp

HEADERS += \
    testanalyseur.h
