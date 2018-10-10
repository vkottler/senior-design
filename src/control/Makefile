.PHONY: clean test
.DEFAULT_GOAL = control.so

%.so: %.c
	gcc -c -fPIC $*.c -o $*.o
	gcc -shared -Wl,-soname,$*.so -o $*.so $*.o

test: control.o test_control.o
	@gcc $^ -o $@
	./$@

clean:
	@find . -name '*.o' -delete
