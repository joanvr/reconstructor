#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T20:58:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = ../CSCLI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH = ../core
OBJECTS_DIR = ../build
SOURCES += cli.cpp \
    ../core/Transforms.cpp \
    ../core/io.cpp \
    ../core/Image.cpp \
    ../core/Reconstructor.cpp
HEADERS += \
    ../core/Transforms.h \
    ../core/datatypes.h \
    ../core/io.h \
    ../core/Image.h \
    ../core/Fista.h \
    ../core/Reconstructor.h \
    ../core/ReconstructorListener.h 
LIBS += -Wl,-V  \
    -lpthread \
    -lm
