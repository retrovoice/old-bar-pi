#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T09:41:39
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weightsAndMeasures
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    catalogue.cpp \
    commandlineparser.cpp \
    databasedialog.cpp \
    productdialog.cpp

HEADERS  += mainwindow.h \
    catalogue.h \
    appStructs.h \
    commandlineparser.h \
    connection.h \
    databasedialog.h \
    productdialog.h
