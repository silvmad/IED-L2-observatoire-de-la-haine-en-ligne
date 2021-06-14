QT += testlib sql
QT += gui
CONFIG += qt warn_on depend_includepath testcase
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

INCLUDEPATH += ../../integration_src

SOURCES += \ 
    testanalyserconfwindow.cpp \
    ../../integration_src/analyserconfwindow.cpp \
    ../../integration_src/patternwindow.cpp \
    ../../integration_src/typeswindow.cpp

HEADERS += \
    testanalyserconfwindow.h \
    ../../integration_src/analyserconfwindow.h \
    ../../integration_src/patternwindow.h \
    ../../integration_src/typeswindow.h
