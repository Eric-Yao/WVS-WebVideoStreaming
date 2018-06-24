##±‡“Î¡¥π§æﬂ
#CROSS       := /home/ui/toolchain/bin/arm-elf-
CROSS      :=
AR          := $(CROSS)ar
AS          := $(CROSS)as
#CC          := $(CROSS)gcc -DHAVE_CONFIG_H -arch i386
#CPP         := $(CROSS)g++ -DHAVE_CONFIG_H -arch i386

#CC          := /Developer/usr/bin/llvm-gcc-4.2 -arch i386
#CPP         := /Developer/usr/bin/llvm-g++-4.2 -arch i386

#CC           := /Developer/usr/bin/gcc -arch i386 -arch x86_64
#CPP          := /Developer/usr/bin/g++ -arch i386 -arch x86_64

CPP           :=g++ #-arch x86_64
CC            :=g++ #-arch x86_64

# -arch i386 -arch x86_64
#CPP         := $(CROSS)g++ -arch i386
CXX         := $(CROSS)g++
GXX         := $(CROSS)g++
MAC_GCC     := $(CROSS)g++-4.2
LD          := $(CROSS)ld
RM          := rm
MV	    := mv
NM          := $(CROSS)nm
RANLIB      := $(CROSS)ranlib
STRIP       := $(CROSS)strip
OBJCOPY     := $(CROSS)objcopy
CD     := $(CROSS)cd

#=========Project All===========

##################################################
ALLMODULES := plat tcp texture record gl_window glx_offscreen apis statistics
.PHONY: all test clean
##################################################
USING_OPENCV:= no
USING_MAC:= no
ifeq ($(USING_MAC),yes)
	PROJECT_TOP_DIR := /Users/yong/Documents/GameDev/CloudGaming/Demo/v4
else
	PROJECT_TOP_DIR := /home/yong/cloudgaming/v4
endif

LIB_DIR = $(PROJECT_TOP_DIR)/libs
LIBSYS_DIR = $(PROJECT_TOP_DIR)/libs_sys
##################################################
MAKE_LIB_DIR = $(PROJECT_TOP_DIR)/libs
MAKE_LIBSYS_DIR = $(PROJECT_TOP_DIR)/libs_sys
##################################################
PLAT_LIBNAME 	 	 = $(LIB_DIR)/libplat.a
TCP_LIBNAME 	 	 = $(LIB_DIR)/libtcp.a
TEXTURE_LIBNAME      = $(LIB_DIR)/libtexture.a
RECORD_LIBNAME       = $(LIB_DIR)/librecord.a
GLWINDOW_LIBNAME     = $(LIB_DIR)/libglwindow.a
GLXOFFSCREEN_LIBNAME = $(LIB_DIR)/libglxoffscreen.a
STATISTICS_LIBNAME   = $(LIB_DIR)/libstatistics.a
APIS_LIBNAME         = $(LIB_DIR)/libapis.a
##################################################
MAKE_PLAT_LIBNAME 	 	    = $(MAKE_LIB_DIR)/libplat.a
MAKE_TCP_LIBNAME 	 	    = $(MAKE_LIB_DIR)/libtcp.a
MAKE_TEXTURE_LIBNAME 	 	= $(MAKE_LIB_DIR)/libtexture.a
MAKE_RECORD_LIBNAME 	 	= $(MAKE_LIB_DIR)/librecord.a
MAKE_GLWINDOW_LIBNAME 	 	= $(MAKE_LIB_DIR)/libglwindow.a
MAKE_GLXOFFSCREEN_LIBNAME 	= $(MAKE_LIB_DIR)/libglxoffscreen.a
MAKE_STATISTICS_LIBNAME 	= $(MAKE_LIB_DIR)/libstatistics.a
MAKE_APIS_LIBNAME           = $(MAKE_LIB_DIR)/libapis.a

##################################################
ifeq ($(USING_MAC),yes)
	SYSTEM_INCLUDE_PATH = -L/opt/X11/lib/ -I/opt/X11/include/ \
                          -I/usr/local/Cellar/jpeg/8d/include/ \
                          -I/usr/local/include/freetype2/ \
                          -I/usr/local/include/libpng/ \
                          -I/usr/local/include/libpng15/
else
	SYSTEM_INCLUDE_PATH = -L/usr/lib/ \
                          -L/usr/local/lib/ \
                          -L/home/yong/ffmpeg_build/lib/ \
                          -L/home/yong/ffmpeg_build/include/ \
                          -I/usr/include/GL/ \
                          -I/usr/include/X11/ \
                          -I/usr/local/include/
endif

ifeq ($(USING_MAC),yes)
	PROJECT_INCLUDE_PRE_PATH = $(PROJECT_TOP_DIR)/engine/include/mac/
else
	PROJECT_INCLUDE_PRE_PATH = $(PROJECT_TOP_DIR)/engine/include/linux/
endif

PROJECT_INCLUDE_PATH = -I$(PROJECT_INCLUDE_PRE_PATH) \
                       -I$(PROJECT_TOP_DIR)/engine/tcp/ \
                       -I$(PROJECT_TOP_DIR)/engine/plat/ \
                       -I$(PROJECT_TOP_DIR)/engine/texture/ \
                       -I$(PROJECT_TOP_DIR)/engine/record/ \
                       -I$(PROJECT_TOP_DIR)/engine/gl_window/ \
                       -I$(PROJECT_TOP_DIR)/engine/glx_offscreen/ \
                       -I$(PROJECT_TOP_DIR)/engine/apis/ \
                       -I$(PROJECT_TOP_DIR)/engine/statistics/ \
##################################################
OUTPUT_FILE = test
PROJECT_OBJ_DIR := $(PROJECT_TOP_DIR)/libs/objs
PROJECT_INCS = -I./ $(SYSTEM_INCLUDE_PATH) $(PROJECT_INCLUDE_PATH)
##################################################
DEBUG:=no

#ifeq ($(DEBUG),yes)
#	PROJECT_CFLAGS := `pkg-config --cflags opencv` -O -g
#else
#	PROJECT_CFLAGS := `pkg-config --cflags opencv` -O
#endif

ifeq ($(DEBUG),yes)
	PROJECT_CFLAGS := -O -g
else
	PROJECT_CFLAGS := -O
endif

PROJECT_LDFLAGS := `pkg-config --libs opencv`
##################################################
COMPILE_STATUS :=@echo "<==== gcc is compiling $< ====>"
ARLIB_STATUS :=@echo "<==== gcc is creating $@  ====>"
RMOBJ_STATUS :=@echo "<==== gcc is removing $^  ====>"
##################################################
