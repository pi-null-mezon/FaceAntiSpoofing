QT += network

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

DEFINES += APP_NAME=\\\"$${TARGET}\\\"

SOURCES += \
        main.cpp \
    qtaskserver.cpp \
    qimagesearcher.cpp \
    qvideosource.cpp \
    qimagesaver.cpp

include($${PWD}/opencv.pri)

INCLUDEPATH += $${PWD}/../Protocol

HEADERS += \
    qtaskserver.h \
    qimagesearcher.h \
    qvideosource.h \
    qimagesaver.h
