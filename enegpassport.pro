#-------------------------------------------------
#
# Project created by QtCreator 2016-02-08T10:54:18
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = enegpassport
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treeitem.cpp \
    treemodel.cpp \
    energypassportmodel.cpp \
    pakazatel.cpp \
    dbctx.cpp \
    fragment.cpp \
    section.cpp \
    entity.cpp \
    dialogteplozashita.cpp

HEADERS  += mainwindow.h \
    treeitem.h \
    treemodel.h \
    energypassportmodel.h \
    pakazatel.h \
    dbctx.h \
    fragment.h \
    section.h \
    entity.h \
    dialogteplozashita.h

FORMS    += mainwindow.ui \
    dialogteplozashita.ui

RESOURCES += \
    enegresources.qrc

DISTFILES +=