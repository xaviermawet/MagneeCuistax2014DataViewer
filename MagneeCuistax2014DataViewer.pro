#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T20:40:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagneeCuistax2014DataViewer
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Utils/NException.cpp

HEADERS  += MainWindow.hpp \
    Utils/NException.hpp

FORMS    += MainWindow.ui

RESOURCES += \
    Resources.qrc
