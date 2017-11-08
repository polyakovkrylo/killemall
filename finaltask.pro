#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T14:12:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = finaltask
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp

HEADERS  += \
    libworld/world_global.h \
    libworld/world.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libworld/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libworld/debug/ -lworld
else:unix: LIBS += -L$$PWD/libworld/ -lworld

INCLUDEPATH += $$PWD/libworld
DEPENDPATH += $$PWD/libworld

DISTFILES += \
    libworld/libworld.so \
    libworld/libworld.so.1 \
    libworld/libworld.so.1.0 \
    libworld/libworld.so.1.0.0
