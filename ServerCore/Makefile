#***********************************************************
#	
#	EB
#
#	AUTHOR:		norhustar@gmail.com
#	PURPOSE:	
#	CREATED:	2013-04-23   
#
#	MODIFICATION HISTORY
#	Date        By     Details
#	----------  -----  -------
#
#***********************************************************
include ./script/projectcfg.mak

ifeq ($(USING_MAC),yes)
	LIBS_OPENGL := -framework OpenGL -framework GLUT -lGL -lx11 -lpthread -lstdc++
else
	LIBS_OPENGL := -lglut -lGL -lX11 -lGLU -lpthread -lstdc++
endif

LIBS_IMAGE = -lfreetype -lpng12 -lpng -ljpeg
LIBS_FFMPEG = -lm -lz -lpthread -lavformat -lavcodec -lavutil -lx264 -lx264
LIBS_OPENCV = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect

ifeq ($(USING_OPENCV),yes)
	SYSTEM_LIBS	:= $(LIBS_IMAGE) $(LIBS_FFMPEG) $(LIBS_OPENCV) $(LIBS_OPENGL)
else
	SYSTEM_LIBS	:= $(LIBS_IMAGE) $(LIBS_OPENGL)
endif

PROJ_LIBS_A	:= $(MAKE_APIS_LIBNAME) $(MAKE_GLXOFFSCREEN_LIBNAME) $(MAKE_GLWINDOW_LIBNAME) $(MAKE_STATISTICS_LIBNAME)
PROJ_LIBS_B	:= $(MAKE_RECORD_LIBNAME) $(MAKE_TEXTURE_LIBNAME) $(MAKE_TCP_LIBNAME) $(MAKE_PLAT_LIBNAME)

#$(MAKE_STATISTICS_LIBNAME) 

CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`

PROJ_LIBS	:= $(PROJ_LIBS_A) $(PROJ_LIBS_B)
INCLUDES	:= $(PROJECT_INCS)
TARGET		:= $(OUTPUT_FILE)

vpath %.cpp  ./main/

$(TARGET):main.cpp $(LIBS)
		$(CPP) $(PROJECT_CFLAGS) ./main/main.cpp $(INCLUDES) -L. $(PROJ_LIBS) $(SYSTEM_LIBS) -g -o $(TARGET)

.PHONY : clean
clean :
	rm -f *.o
	rm -f $(TARGET)
