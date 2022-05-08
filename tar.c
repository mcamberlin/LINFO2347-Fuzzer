#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc, calloc, free

#include "tar.h"

#define ERROR(descr, ...) fprintf(stderr, "Error: " descr "\n", ##__VA_ARGS__);

// =============================================

/**
 * Create a tar file with name @tar_name with one file entry (header + file)
 * @param tar_name: The name of the tar archive to create
 * @param header: The tar header to write
 * @param file: The file to put into the created tar
 * @return -1 if the process failed
 *          0 if case of success
 */
int tar_write(const char* tar_name, const struct tar_t* header, const char* file)
{

    // file creation
    FILE* archive = NULL;

    if ( (archive = fopen( tar_name, "w+") ) == NULL)
    {
        ERROR("Unable to creation the tar file");
        return -1;
    }
    
    // file entry creation
    // write header
    int rslt;
    if( (rslt = fwrite(header, sizeof(struct tar_t), 1, archive)) != 1 )
    {
        ERROR("Unable to write header");
        return -1;
    }

    // file entry creation
    // write file into archive
    if( (rslt = fwrite(file, sizeof(file),1, archive)) != 1 )
    {
        ERROR("Unable to write file");
        return -1;
    } 

    // add padding bytes
    size_t padding = 512 - (sizeof(file) % 512);
    char c = '0';

    if( (rslt = fwrite( &c, 1, padding, archive)) != (int) padding )
    {
        ERROR("Unable to write padding");
        return -1;
    }
    
    // add end-of-archive marker = two 512-byte blocks of zero bytes
    char* end_of_archive;
    if( (end_of_archive = (char *) calloc(1024, sizeof(char))) == NULL )
    {
        ERROR("Unable to calloc end_of_archive");
        return -1;
    }

    if( (rslt = fwrite(end_of_archive, 1024, 1, archive) ) != 1 )
    {
        ERROR("Unable to write end-of-archive");
        return -1;
    }

    if( fclose(archive) != 0) 
    {
        ERROR("Unable to close");
        return -1;
    }

    return 0;
}

