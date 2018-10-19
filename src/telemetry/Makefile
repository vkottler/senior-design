.PHONY: clean
.DEFAULT_GOAL = test

TELEMETRY_DEPS  = channel.o
TELEMETRY_DEPS += manifest.o
TELEMETRY_DEPS += packet.o

CFLAGS = -std=c99 -Werror -pedantic -m32

# compile position-independent for libraries
%.o: %.c
	gcc -c $(CFLAGS) -fPIC $*.c -o $*.o

# static library
libtelemetry.a: $(TELEMETRY_DEPS)
	ar rcs $@ $^

# shared library
libtelemetry.so: $(TELEMETRY_DEPS)
	gcc -shared -Wl,-soname,$@ -o $@ $^

#export LD_LIBRARY_PATH = $(shell pwd)
#	gcc $< -L$(shell pwd) -ltelemetry -o $@
test: test.o libtelemetry.a
	gcc $(CFLAGS) $^ -o $@

run: test
	@./test

clean:
	@find . -name '*.o' -delete
	@rm -f test
