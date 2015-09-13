#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T13:38:38
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = PloucsMessengerServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    threadclient.cpp

HEADERS += \
    server.h \
    threadclient.h
