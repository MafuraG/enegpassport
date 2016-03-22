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
    t1t2dialog.cpp \
    excelreport.cpp \
    sn50pg18dialog.cpp \
    buildingtype.cpp

HEADERS  += mainwindow.h \
    treeitem.h \
    treemodel.h \
    energypassportmodel.h \
    pakazatel.h \
    dbctx.h \
    fragment.h \
    section.h \
    entity.h \
    t1t2dialog.h \
    excelreport.h \
    sn50pg18dialog.h \
    buildingtype.h

FORMS    += mainwindow.ui \
    t1t2dialog.ui \
    sn50pg18dialog.ui \
    buildingtype.ui

RESOURCES += \
    enegresources.qrc

DISTFILES +=

include(3rdparty/QtXlsxWriter/src/xlsx/qtxlsx.pri)
