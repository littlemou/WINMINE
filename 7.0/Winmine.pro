#-------------------------------------------------
#
# Project created by QtCreator 2020-09-09T16:04:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Winmine
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


SOURCES += \
        main.cpp \
        winmine.cpp \
    block.cpp \
    customize_window.cpp \
    dialog.cpp \
    rank.cpp \
    winmine2.cpp

HEADERS += \
        winmine.h \
    block.h \
    customize_window.h \
    dialog.h \
    rank.h \
    winmine2.h

FORMS += \
        winmine.ui \
    customize_window.ui \
    dialog.ui \
    rank.ui \
    winmine2.ui

RESOURCES += \
    images.qrc