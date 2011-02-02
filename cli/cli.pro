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
    ../core/Matrix.cpp \
    ../core/io.cpp \
    ../core/Image.cpp \
    ../core/Dantzig.cpp \
    ../core/Reconstructor.cpp
HEADERS += \
    ../core/Transforms.h \
    ../core/Matrix.h \
    ../core/io.h \
    ../core/Image.h \
    ../core/Dantzig.h \
    ../core/Reconstructor.h \
    ../core/ReconstructorListener.h \
    ../core/gurobi_c++.h \
    ../core/gurobi_c.h
LIBS += -Wl,-V -L$(GUROBI_HOME)/lib/ \
    -lgurobi_c++ \
    -lgurobi40 \
    -lpthread \
    -lm
