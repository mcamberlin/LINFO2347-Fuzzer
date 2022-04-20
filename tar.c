#include "tar.h"

#include <stdio.h>

int error(descr)
{
    fprintf(stderr, "Error: " descr "\n"); 
    return -1;
}

// =============================================

/**
 * Create a tar file with a header and a content
 * @param header: The tar header to write
 * @param file: the content of the file to put on a tar
 */
int tar_write(struct tar_t* header, char* content)
{
    FILE* file = NULL;

    if ( (file = fopen("archive.tar", "w+") ) == NULL)
    {
        error("Unable to open the tar file");
    }

    // header
    if( fwrite(header, sizeof(struct tar_t), 1, file) != 512 )
    {
        error("Unable to write header \n");
    } 

    // content
    if( fwrite(content, strlen(file),1, file) != strlen(file) )
    {
        error("Unable to write file \n");
    } 

    // padding
    int padding = 512 - (strlen(file) % 512);
    char c = "0"
    if( fwrite((const void*) &c, sizeof(char), padding, file) != padding )
    {
        error("Unable to write padding \n");
    }
    
    // end-of-archive marker (two 512-byte blocks filled with zeros)
    char* end_of_archive = (char *) calloc(1024, sizeof(char));
    if( end_of_archive == NULL)
    {
        error("Unable to calloc archive");
    }

    if( fwrite(end_of_archive, 1024 * sizeof(char), 1, file) != 1024 )
    {
        error("Unable to write end-of-archive \n");
    }

    if( fclose(file) != 0) 
    {
        error("Unable to close file \n");
    }

    return 0;
}

