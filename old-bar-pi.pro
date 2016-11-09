#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T09:41:39
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = barpi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    commandlineparser.cpp \
    databasedialog.cpp \
    catalog.cpp

HEADERS  += mainwindow.h \
    commandlineparser.h \
    databasedialog.h \
    catalog.h
