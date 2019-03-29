TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include($${PWD}/../../../Kaggle/Shared/dlib.pri)
include($${PWD}/../../../Kaggle/Shared/opencv.pri)

HEADERS += customnetwork.h

INCLUDEPATH += $${PWD}/../../../Kaggle/Shared/dlibimgaugment \
               $${PWD}/../../../Kaggle/Shared/opencvimgaugment \
               $${PWD}/../../../Kaggle/Shared/dlibopencvconverter

unix {
   target.path = /usr/local/bin
   INSTALLS += target
}
