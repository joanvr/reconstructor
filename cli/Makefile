#############################################################################
# Makefile for building: ../CSCLI
# Generated by qmake (2.01a) (Qt 4.6.2) on: dt. gen. 25 16:54:13 2011
# Project:  cli.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -unix -o Makefile cli.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG
CFLAGS        = -pipe -O3 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O3 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4 -I../core -I.
LINK          = g++
LFLAGS        = -Wl,-O3 
LIBS          = $(SUBLIBS)  -L/usr/lib -Wl,-V -L$(GUROBI_HOME)/lib/ -lgurobi_c++ -lgurobi40 -lm -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ../build/

####### Files

SOURCES       = cli.cpp \
		../core/Transforms.cpp \
		../core/Matrix.cpp \
		../core/io.cpp \
		../core/Image.cpp \
		../core/Dantzig.cpp \
		../core/Reconstructor.cpp 
OBJECTS       = ../build/cli.o \
		../build/Transforms.o \
		../build/Matrix.o \
		../build/io.o \
		../build/Image.o \
		../build/Dantzig.o \
		../build/Reconstructor.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		cli.pro
QMAKE_TARGET  = CSCLI
DESTDIR       = ../
TARGET        = ../CSCLI

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) ../ || $(MKDIR) ../ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: cli.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -unix -o Makefile cli.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile cli.pro

dist: 
	@$(CHK_DIR_EXISTS) ../build/CSCLI1.0.0 || $(MKDIR) ../build/CSCLI1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) ../build/CSCLI1.0.0/ && $(COPY_FILE) --parents ../core/Transforms.h ../core/Matrix.h ../core/io.h ../core/Image.h ../core/Dantzig.h ../core/Reconstructor.h ../core/ReconstructorListener.h ../core/gurobi_c++.h ../core/gurobi_c.h ../build/CSCLI1.0.0/ && $(COPY_FILE) --parents cli.cpp ../core/Transforms.cpp ../core/Matrix.cpp ../core/io.cpp ../core/Image.cpp ../core/Dantzig.cpp ../core/Reconstructor.cpp ../build/CSCLI1.0.0/ && (cd `dirname ../build/CSCLI1.0.0` && $(TAR) CSCLI1.0.0.tar CSCLI1.0.0 && $(COMPRESS) CSCLI1.0.0.tar) && $(MOVE) `dirname ../build/CSCLI1.0.0`/CSCLI1.0.0.tar.gz . && $(DEL_FILE) -r ../build/CSCLI1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

../build/cli.o: cli.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/cli.o cli.cpp

../build/Transforms.o: ../core/Transforms.cpp ../core/Transforms.h \
		../core/Matrix.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/Transforms.o ../core/Transforms.cpp

../build/Matrix.o: ../core/Matrix.cpp ../core/Matrix.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/Matrix.o ../core/Matrix.cpp

../build/io.o: ../core/io.cpp ../core/io.h \
		../core/Matrix.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/io.o ../core/io.cpp

../build/Image.o: ../core/Image.cpp ../core/Image.h \
		../core/Matrix.h \
		../core/io.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/Image.o ../core/Image.cpp

../build/Dantzig.o: ../core/Dantzig.cpp ../core/Dantzig.h \
		../core/Matrix.h \
		../core/gurobi_c++.h \
		../core/gurobi_c.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/Dantzig.o ../core/Dantzig.cpp

../build/Reconstructor.o: ../core/Reconstructor.cpp ../core/Reconstructor.h \
		../core/Image.h \
		../core/Matrix.h \
		../core/io.h \
		../core/ReconstructorListener.h \
		../core/gurobi_c++.h \
		../core/gurobi_c.h \
		../core/Transforms.h \
		../core/Dantzig.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/Reconstructor.o ../core/Reconstructor.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

