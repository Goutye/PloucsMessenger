#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T12:56:39
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PloucsMessenger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socketclient.cpp

HEADERS  += mainwindow.h \
    socketclient.h
