#QT += core sql testlib

TEMPLATE = subdirs
SUBDIRS += analyseur integration test

test.depends = analyseur

CONFIG += ordered
#CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt

#SOURCES += \
#        main.cpp \
#    testanalyseur.cpp
#
#HEADERS += \
#    main.h \
#    testanalyseur.h
