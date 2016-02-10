#-------------------------------------------------
#
# Project created by QtCreator 2016-02-08T10:54:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = enegpassport
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treeitem.cpp \
    treemodel.cpp \
    energypassportmodel.cpp \
    measure.cpp

HEADERS  += mainwindow.h \
    treeitem.h \
    treemodel.h \
    energypassportmodel.h \
    measure.h

FORMS    += mainwindow.ui

RESOURCES += \
    enegresources.qrc

DISTFILES +=
