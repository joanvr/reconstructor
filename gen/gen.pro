#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T20:58:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = ../CSGEN
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH = ../core
OBJECTS_DIR = ../build
SOURCES += gen.cpp \
    ../core/Matrix.cpp \
    ../core/io.cpp \
    ../core/Image.cpp 
HEADERS += \
    ../core/Matrix.h \
    ../core/io.h \
    ../core/Image.h

