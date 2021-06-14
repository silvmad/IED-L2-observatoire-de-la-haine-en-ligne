QT += testlib sql
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \ 
    testtypeswindow.cpp \
    ../../integration_src/analyserconfwindow.cpp \
    ../../integration_src/typeswindow.cpp \
    ../../integration_src/patternwindow.cpp

HEADERS += \
    testtypeswindow.h \
    ../../integration_src/analyserconfwindow.h \
    ../../integration_src/typeswindow.h \
    ../../integration_src/patternwindow.h
