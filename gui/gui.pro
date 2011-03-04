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
    ../core/io.cpp \
    ../core/Image.cpp \
    ../core/Reconstructor.cpp
HEADERS += widget.h \
    ../core/Transforms.h \
    ../core/datatypes.h \
    ../core/io.h \
    ../core/Image.h \
    ../core/Fista.h \
    ../core/Reconstructor.h \
    ../core/ReconstructorListener.h 
FORMS += widget.ui
LIBS += -lpthread \
    -lm
