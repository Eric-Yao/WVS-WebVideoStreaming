include ../../script/projectcfg.mak

CFLAGS		= $(PROJECT_CFLAGS)
INCLUDES	= $(PROJECT_INCS)

OBJ		:= draw.o draw_apis.o
		   
TARGET		:= $(GLWINDOW_LIBNAME)

%o: %c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	
	
$(TARGET): $(OBJ)
	$(ARLIB_STATUS)
	$(AR) -rcs $(TARGET) $^
	$(MV) $^ $(PROJECT_OBJ_DIR)/

.PHONY:	clean
clean:
	$(RM) -f $(PROJECT_OBJ_DIR)/*.o	