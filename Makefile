# See gcc/clang manual to understand all flags
CFLAGS += -std=gnu99 # Define which version of the C standard to use
CFLAGS += -Wall # Enable the 'all' set of warnings
CFLAGS += -Wshadow # Warn when shadowing variables
CFLAGS += -Wextra # Enable additional warnings

# Default target
all: fuzzer

fuzzer : 
	gcc -o fuzzer tar.c fuzzer.c -lz $(CFLAGS)
	./fuzzer

clean:
	@rm -f fuzzer