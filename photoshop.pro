#-------------------------------------------------
#
# Project created by QtCreator 2018-12-16T00:34:24
#
#-------------------------------------------------

QT       += core gui widgets\
            printsupport

TARGET = photoshop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    imageprocessor.cpp

HEADERS += \
        mainwindow.h \
    imageprocessor.h

FORMS += \
        mainwindow.ui

TARGET = app
RESOURCES = breeze.qrc

INCLUDEPATH += D:/program/openCV/opencv/build/include \

LIBS += D:/program/openCV/opencv/build/x64/vc15/lib/opencv_world343.lib  \
        D:/program/openCV/opencv/build/x64/vc15/lib/opencv_world343d.lib  \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
