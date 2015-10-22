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
    soundmanager.cpp \
    inputtextedit.cpp \
    Tabs/tabsarea.cpp \
    Tabs/chatwidget.cpp \
    Tabs/displaychat.cpp \
    Options/windowbuttons.cpp \
    Options/optionbutton.cpp \
    Options/useraction.cpp \
    emoticons.cpp \
    emoticondownloader.cpp \
    notification.cpp \
    SizeGrip/hsizegrip.cpp \
    SizeGrip/vsizegrip.cpp \
    SizeGrip/fittedhsizegrips.cpp \
    Tabs/pushbutton.cpp

HEADERS  += mainwindow.h \
    socketclient.h \
    filedownloader.h \
    soundmanager.h \
    inputtextedit.h \
    Tabs/chatwidget.h \
    Tabs/displaychat.h \
    Options/optionbutton.h \
    Options/windowbuttons.h \
    Options/useraction.h \
    emoticons.h \
    emoticondownloader.h \
    notification.h \
    SizeGrip/hsizegrip.h \
    SizeGrip/vsizegrip.h \
    SizeGrip/fittedhsizegrips.h \
    Tabs/tabsarea.h \
    Tabs/pushbutton.h

RESOURCES += \
    icons.qrc \
    sounds.qrc
