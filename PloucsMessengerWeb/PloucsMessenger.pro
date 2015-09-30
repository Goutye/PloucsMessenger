#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T12:56:39
#
#-------------------------------------------------

QT       += core gui network multimedia
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PloucsMessenger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socketclient.cpp \
    filedownloader.cpp \
    useraction.cpp \
    displaychat.cpp \
    tabsarea.cpp \
    chatwidget.cpp \
    tabbar.cpp \
    soundmanager.cpp \
    inputtextedit.cpp \
    optionbutton.cpp \
    windowbuttons.cpp

HEADERS  += mainwindow.h \
    socketclient.h \
    filedownloader.h \
    useraction.h \
    displaychat.h \
    tabsarea.h \
    chatwidget.h \
    tabbar.h \
    soundmanager.h \
    inputtextedit.h \
    optionbutton.h \
    windowbuttons.h

RESOURCES += \
    icons.qrc
