.PHONY: clean test
.DEFAULT_GOAL = test

TELEMETRY_DEPS  = channel.o
TELEMETRY_DEPS += manifest.o
TELEMETRY_DEPS += packet.o

%.o: %.c
	gcc -c -fPIC $*.c -o $*.o

libtelemetry.a: $(TELEMETRY_DEPS)
	ar rcs $@ $^

test: test.o libtelemetry.a
	gcc $^ -o $@
	./$@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete
	@rm -f test

#%.o: %.c
#	gcc -c -fPIC $*.c -o $*.o
#$(TELEM_LIB): $(TELEMETRY_DEPS)
#	gcc -shared -Wl,-soname,$@ -o $@ $^
#export LD_LIBRARY_PATH = $(shell pwd)
#test: telemetry_test.o $(TELEM_LIB)
#	gcc $< -L$(shell pwd) -ltelemetry -o $@
#	./$@
