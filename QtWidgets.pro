#-------------------------------------------------
#
# Project created by QtCreator 2016-09-24T08:37:27
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtWidgets
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    matrix.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    matrix.h

FORMS    += mainwindow.ui
QT       += printsupport
