DIR_BLD = bld
DIR_SRC = src
DIR_INSTALL_INC = /usr/local/include/argent
DIR_INSTALL_LIB = /usr/local/lib


LIB_API = middleware.h
LIB_INP = $(DIR_BLD)/log.o \
          $(DIR_BLD)/mpool.o
LIB_OUT = libargent-middleware.so


$(DIR_BLD)/$(LIB_OUT): $(LIB_INP)
	gcc -shared -g -O2 $(LIB_INP) -o $@


$(DIR_BLD)/%.o: $(DIR_SRC)/%.c
	mkdir -p $(DIR_BLD)
	gcc -c -fPIC $(LIB_OPT) $< -o $@


install:
	sudo mkdir -p $(DIR_INSTALL_INC)
	sudo mkdir -p $(DIR_INSTALL_LIB)
	sudo cp $(DIR_SRC)/$(LIB_API)  $(DIR_INSTALL_INC)
	sudo cp $(DIR_BLD)/$(LIB_OUT) $(DIR_INSTALL_LIB)
	sudo ldconfig


uninstall:
	sudo rm -rf $(DIR_INSTALL_INC)/$(LIB_API)
	sudo rm -f $(DIR_INSTALL_LIB)/$(LIB_OUT)


clean:
	rm -rfv $(DIR_BLD)

