#-------------------------------------------------
#
# Project created by James Lawley 2017-12-19T12:21:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPServer
TEMPLATE = app

include("TCP/tcp.pri")

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        dialog.cpp

HEADERS += \
        dialog.h

FORMS += \
        dialog.ui
