LIB_INP = bld/mpool.o
LIB_OUT = bld/libargent-middleware.so
LIB_OPT = -shared -g -O2

$(LIB_OUT): $(LIB_INP)
	gcc $(LIB_OPT) $(LIB_INP) -o $@


bld/%.o: src/%.c
	mkdir -p bld
	gcc -c -fPIC $(LIB_OPT) $< -o $@


clean:
	rm -rfv bld

