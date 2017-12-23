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


SOURCES += main.cpp \
    model/worldmodel.cpp \
    graphicsview/worldgraphicsview.cpp \
    controller/worldabstractcontroller.cpp \
    controller/worldcontrollerfactory.cpp \
    controller/astarcontroller.cpp \
    libworld-update/uworld.cpp \
    terminalview/worldterminalview.cpp \
    mainui/mainwindow.cpp \
    mainui/popup.cpp \
    strategy/worldstrategy.cpp \
    graphicsview/worldgraphicsscene.cpp
HEADERS += \
           ../libworld/world.h \
    model/worldmodel.h \
    graphicsview/worldgraphicsview.h \
    controller/worldabstractcontroller.h \
    controller/worldcontrollerfactory.h \
    controller/astarcontroller.h \
    libworld-update/uworld.h \
    terminalview/worldterminalview.h \
    mainui/mainwindow.h \
    mainui/popup.h \
    strategy/worldstrategy.h \
    graphicsview/worldgraphicsscene.h


LIBS += -L../libworld/ -lworld
INCLUDEPATH += ../libworld/
DEPENDPATH += ../libworld/

DISTFILES += \
    finaltask.qmodel

RESOURCES += \
    res.qrc

FORMS += \
    mainui/mainwindow.ui \
    mainui/popup.ui

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
