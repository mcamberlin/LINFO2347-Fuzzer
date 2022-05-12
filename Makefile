CFLAGS += -std=gnu99 	# Define which version of the C standard to use
CFLAGS += -Wall 		# Enable the 'all' set of warnings
CFLAGS += -Werror 		# Convert every warning into errors
CFLAGS += -Wshadow 		# Warn when shadowing variables
CFLAGS += -Wextra 		# Enable additional warnings

# 
all: fuzzer

fuzzer : 
	gcc -o fuzzer src/help.c src/tar.c src/fuzzer.c -lz $(CFLAGS)
run:
	@rm -f fuzzer
	gcc -o fuzzer src/help.c src/tar.c src/fuzzer.c -lz $(CFLAGS)
	./fuzzer ./extractor
	
# rm !(Makefile|extractor|*.tar) to clean the folder
clean:
	@rm -f fuzzer
	@rm -f name
	@rm -f mode
	@rm -f uid
	@rm -f gid
	@rm -f size
	@rm -f mtime
	@rm -f checksum
	@rm -f typeflag
	@rm -f linkname
	@rm -f magic
	@rm -f version
	@rm -f uname
	@rm -f gname
	@rm -f header_no_data
	@rm -f data_content
	@rm -f archive.tar
	@clear