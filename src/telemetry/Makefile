.PHONY: clean test
.DEFAULT_GOAL = test

TELEMETRY_DEPS  = channel.o
TELEMETRY_DEPS += manifest.o
TELEMETRY_DEPS += packet.o

# compile position-independent for libraries
%.o: %.c
	gcc -c -fPIC $*.c -o $*.o

# static library
libtelemetry.a: $(TELEMETRY_DEPS)
	ar rcs $@ $^

# shared library
libtelemetry.so: $(TELEMETRY_DEPS)
	gcc -shared -Wl,-soname,$@ -o $@ $^

#export LD_LIBRARY_PATH = $(shell pwd)
#	gcc $< -L$(shell pwd) -ltelemetry -o $@
test: test.o libtelemetry.a
	gcc $^ -o $@
	./$@

clean:
	@find . -name '*.o' -delete
	@rm -f test
