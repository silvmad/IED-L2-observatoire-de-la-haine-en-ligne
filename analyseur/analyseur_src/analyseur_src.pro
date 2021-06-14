QT += core gui sql

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

INCLUDEPATH += ../../integration/integration_src

SOURCES += \
    main.cpp \
    func.cpp \
    ../../integration/integration_src/conflist.cpp

HEADERS += \
    main.h
