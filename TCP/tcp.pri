QT += network
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += \
    $$PWD/tcpserver.h \
    $$PWD/tcpconnections.h \
    $$PWD/tcpconnection.h

SOURCES += \
    $$PWD/tcpserver.cpp \
    $$PWD/tcpconnections.cpp \
    $$PWD/tcpconnection.cpp
