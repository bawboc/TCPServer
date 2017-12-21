QT += network
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/httpserver.h \
    $$PWD/httpconnection.h

SOURCES += \
    $$PWD/httpserver.cpp \
    $$PWD/httpconnection.cpp
