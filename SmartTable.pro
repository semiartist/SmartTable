#-------------------------------------------------
#
# Project created by QtCreator 2016-10-25T15:46:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartTable
TEMPLATE = app


SOURCES += main.cpp \
    Button.cpp \
    PhotoDisplay.cpp \
    TextDisplay.cpp \
    WorkTable.cpp \
    ApplicationArea.cpp \
    Dialog.cpp \
    OpenCVWindow.cpp

HEADERS  += \
    Button.h \
    PhotoDisplay.h \
    TextDisplay.h \
    WorkTable.h \
    ApplicationArea.h \
    Dialog.h \
    OpenCVWindow.h

FORMS    += \
    Dialog.ui \
    OpenCVWindow.ui

RESOURCES += \
    Res.qrc

LIBS += -L/usr/local/lib -L/usr/local/share/OpenCV/3rdparty/lib/
CONFIG  += link_pkgconfig
PKGCONFIG += opencv
