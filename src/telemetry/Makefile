.PHONY: telemetry clean test

%.so: %.c
	gcc -c -fPIC $*.c -o $*.o
	gcc -shared -Wl,-soname,$*.so -o $*.so $*.o

telemetry: telemetry.so

test: telemetry.o telemetry_test.o
	@gcc $^ -o $@
	./$@

clean:
	@find . -name '*.o' -delete
	@rm -f telemetry.so
