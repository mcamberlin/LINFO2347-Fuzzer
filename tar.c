#include "tar.h"
#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc, calloc, free

#define ERROR(descr, ...) fprintf(stderr, "Error: " descr "\n", ##__VA_ARGS__); return -1;

// =============================================

/**
 * Create a tar file with one file entry (header + file)
 * @param tar_name: The name of the tar archive to create
 * @param header: The tar header to write
 * @param file: the file to put into the created tar
 */
int tar_write(const char* tar_name, const struct tar_t* header, const char* file)
{
    FILE* archive = NULL;

    // file creation
    if ( (archive = fopen( tar_name, "w+") ) == NULL)
    {
        ERROR("Unable to creation the tar file");
    }
    
    // file entry creation
    // write header
    if( fwrite(header, sizeof(header), 1, archive) != 512 )
    {
        ERROR("Unable to write header");
    } 
    // file entry creation
    // write file into archive
    if( fwrite(file, sizeof(file),1, archive) != sizeof(file) )
    {
        ERROR("Unable to write file");
    } 

    // add padding bytes
    size_t padding = 512 - (sizeof(file) % 512);
    char* c = "0";

    if( fwrite(c, 1, padding, archive) != padding )
    {
        ERROR("Unable to write padding");
    }
    
    // add end-of-archive marker = two 512-byte blocks of zero bytes
    char* end_of_archive = (char *) calloc(1024, sizeof(char));
    if( end_of_archive == NULL)
    {
        ERROR("Unable to calloc end_of_archive");
    }

    if( fwrite(end_of_archive, sizeof(end_of_archive), 1, (void*) file) != 1024 )
    {
        ERROR("Unable to write end-of-archive");
    }

    if( fclose(archive) != 0) 
    {
        ERROR("Unable to close");
    }

    return 0;
}

