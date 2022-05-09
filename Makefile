# See gcc/clang manual to understand all flags
CFLAGS += -std=gnu99 	# Define which version of the C standard to use
CFLAGS += -Wall 		# Enable the 'all' set of warnings
CFLAGS += -Werror 		# Convert every warning into errors
CFLAGS += -Wshadow 		# Warn when shadowing variables
CFLAGS += -Wextra 		# Enable additional warnings

# Default target
all: fuzzer

fuzzer : 
	gcc -o fuzzer help.c tar.c fuzzer.c -lz $(CFLAGS)
run:
	@rm -f fuzzer
	gcc -o fuzzer help.c tar.c fuzzer.c -lz $(CFLAGS)
	./fuzzer ./extractor
clean:
	@rm -f fuzzer