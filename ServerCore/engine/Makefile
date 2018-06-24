include ../script/projectcfg.mak

all : 
	@echo "Making Project Modules ..."
	@for i in $(ALLMODULES); do \
	  if ! $(MAKE) -C $$i ; then \
	    exit -1; \
	  fi \
	done

clean : 
	@echo "Cleaning Project..."
	@for i in $(ALLMODULES); do \
	  if ! $(MAKE) -C $$i clean ; then \
	    exit -1; \
	  fi \
	done
	rm -f $(LIB_DIR)/*.a