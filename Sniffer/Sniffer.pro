#-------------------------------------------------
#
# Project created by QtCreator 2018-04-12T19:11:22
#
#-------------------------------------------------

QT       += core gui
#INCLUDEPATH += E:\Sniffer\WpdPack\Include
LIBS +=  E:\\Sniffer\\WpdPack\\Lib\\x64\\Packet.lib E:\\Sniffer\\WpdPack\\Lib\\x64\\wpcap.lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sniffer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS\
         _XKEYCHECK_H

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    pcapuse.cpp \
    threadpcap.cpp \
    filter.cpp \
    date.cpp \
    arpcheat.cpp \
    threadarp.cpp

HEADERS  += mainwindow.h \
    state.h \
    pcapuse.h \
    threadpcap.h \
    filter.h \
    date.h \
    arpcheat.h \
    threadarp.h

FORMS    += mainwindow.ui \
    filter.ui \
    date.ui \
    arpcheat.ui
