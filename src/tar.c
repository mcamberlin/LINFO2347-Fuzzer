/**
 * @file tar.c
 * @author Merlin Camberlin (0944-1700), Zoé Schoofs (3502-1700)
 * @brief This file contains the different functions used in fuzzer.c to generate archive.
 * @version 0.1
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc, calloc, free
#include <string.h> // for strlen

#include "tar.h"

#define ERROR(descr, ...) fprintf(stderr, "Error: " descr "\n", ##__VA_ARGS__);

// =============================================

/**
 * Create a tar file with name @tar_name with one file entry (header + file)
 * @param tar_name: The name of the tar archive to create
 * @param header: The tar header to write
 * @param content: The content to put into the created tar
 * @return -1 if the process failed
 *          0 if case of success
 */
int tar_write(const char* tar_name, const struct tar_t* header, const char* content)
{

    // file creation
    FILE* archive;
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

    if(content != NULL)
    {
        // file entry creation
        // write file into archive
        if( (rslt = fwrite(content, strlen(content),1, archive)) != 1 )
        {
            ERROR("Unable to write file");
            return -1;
        } 

        // add padding bytes
        size_t padding = 512 - (strlen(content) % 512);
        char c[2] = "0";
        if( (rslt = fwrite( c, 1, padding, archive)) != (int) padding )
        {
            ERROR("Unable to write padding");
            return -1;
        }
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
        free(end_of_archive);
        return -1;
    }

    if( fclose(archive) != 0) 
    {
        ERROR("Unable to close");
        free(end_of_archive);
        return -1;
    }
    free(end_of_archive);
    
    return 0;
}

/**
 * Create a tar file with name @tar_name with mutliple file entries (header + file)
 * @param tar_name: The name of the tar archive to create
 * @param headers: The tar headers to write
 * @param contents: The contents to put into the created tar
 * @param n: The number of file entries to write into the created tar
 * @return -1 if the process failed
 *          0 if case of success
 */
int tar_write_multiple_files(const char* tar_name, struct tar_t** headers, char** contents, int n)
{
    int rslt;

    // file creation
    FILE* archive;
    if ( (archive = fopen( tar_name, "w+") ) == NULL)
    {
        ERROR("Unable to creation the tar file");
        return -1;
    }
    
    for(int i=0; i< n; i++)
    {
         
        // file entry creation
        // write header
        if(headers[i] != NULL)
        {
            if( (rslt = fwrite(headers[i], sizeof(struct tar_t), 1, archive)) != 1 )
            {
                ERROR("Unable to write %d header", i);
                return -1;
            }
        }

        // file entry creation
        if(contents[i] != NULL)
        {
            
            // write file into archive
            if( (rslt = fwrite(contents[i], strlen(contents[i]),1, archive)) != 1 )
            {
                ERROR("Unable to write file");
                return -1;
            } 

            // add padding bytes
            size_t padding = 512 - (strlen(contents[i]) % 512);
            
            char c[2] = "0";
            if( (rslt = fwrite( c, 1, padding, archive)) != (int) padding )
            {
                ERROR("Unable to write padding");
                return -1;
            }
        }
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
        free(end_of_archive);
        return -1;
    }

    if( fclose(archive) != 0) 
    {
        ERROR("Unable to close");
        free(end_of_archive);
        return -1;
    }
    free(end_of_archive);
    
    return 0;
}


/**
 * Create a tar file with name @tar_name with one file entry (header + file) but without end-of-archive marker
 * @param tar_name: The name of the tar archive to create
 * @param header: The tar header to write
 * @param content: The content to put into the created tar
 * @return -1 if the process failed
 *          0 if case of success
 */
int tar_write_without_end_of_archive(const char* tar_name, const struct tar_t* header, const char* content)
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

    if(content != NULL)
    {
        // file entry creation
        // write file into archive
        if( (rslt = fwrite(content, strlen(content),1, archive)) != 1 )
        {
            ERROR("Unable to write file");
            return -1;
        } 

        // add padding bytes
        size_t padding = 512 - (strlen(content) % 512);
        char c = '0';
        if( (rslt = fwrite( &c, 1, padding, archive)) != (int) padding )
        {
            ERROR("Unable to write padding");
            return -1;
        }
    }

    /*
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
    */
   
    if( fclose(archive) != 0) 
    {
        ERROR("Unable to close");
        return -1;
    }

    return 0;
}

/**
 * Create a tar file with name @tar_name with one header but no data stored
 * @param tar_name: The name of the tar archive to create
 * @param header: The tar header to write
 * @param content: The content to put into the created tar
 * @return -1 if the process failed
 *          0 if case of success
 */
int tar_write_with_header_without_data(const char* tar_name, const struct tar_t* header, const char* content)
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

    if(content != NULL)
    {
        // file entry creation
        // write file into archive
        if( (rslt = fwrite(content, strlen(content),1, archive)) != 1 )
        {
            ERROR("Unable to write file");
            return -1;
        } 

        // add padding bytes
        size_t padding = 512 - (strlen(content) % 512);
        char c = '0';
        if( (rslt = fwrite( &c, 1, padding, archive)) != (int) padding )
        {
            ERROR("Unable to write padding");
            return -1;
        }
    }

    // add end-of-archive marker = two 512-byte blocks of zero bytes
    char* end_of_archive;
    if( (end_of_archive = (char *) calloc(1024, sizeof(char))) == NULL )
    {
        ERROR("Unable to calloc end_of_archive");
        return -1;
    }

    if( (rslt = fwrite(end_of_archive, 1024*sizeof(char), 1, archive) ) != 1 )
    {
        ERROR("Unable to write end-of-archive");
        free(end_of_archive);
        return -1;
    }

    if( fclose(archive) != 0) 
    {
        ERROR("Unable to close");
        free(end_of_archive);
        return -1;
    }
    free(end_of_archive);
    return 0;
}