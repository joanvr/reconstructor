# -------------------------------------------------
# Project created by QtCreator 2010-12-12T02:33:05
# -------------------------------------------------
TARGET = ../CSGUI
TEMPLATE = app
INCLUDEPATH = ../core
OBJECTS_DIR = ../build
SOURCES += gui.cpp \
    widget.cpp \
    ../core/Transforms.cpp \
    ../core/Matrix.cpp \
    ../core/io.cpp \
    ../core/Image.cpp \
    ../core/Dantzig.cpp \
    ../core/Reconstructor.cpp
HEADERS += widget.h \
    ../core/Transforms.h \
    ../core/Matrix.h \
    ../core/io.h \
    ../core/Image.h \
    ../core/Dantzig.h \
    ../core/Reconstructor.h \
    ../core/ReconstructorListener.h \
    ../core/gurobi_c++.h \
    ../core/gurobi_c.h
FORMS += widget.ui
LIBS += -L$(GUROBI_HOME)/lib/ \
    -lgurobi_c++ \
    -lgurobi40 \
    -lpthread \
    -lm
