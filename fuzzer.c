#include <stdio.h> // for printf, fprintf
#include <stdlib.h> // for malloc, calloc, free
#include <string.h> // for strncpy, memset, strlen
#include <sys/stat.h> // for size of file
#include <sys/types.h> // for size of file

// https://ascii-tables.com/

#include "tar.h"
#include "help.h"

#define ERROR(descr, ...) fprintf(stderr, "Error: " descr "\n", ##__VA_ARGS__);

/**
        strcpy(header->name     , "");
        strcpy(header->mode     , "");
        strcpy(header->uid      , "");
        strcpy(header->gid      , "");
        strcpy(header->size     , "");
        strcpy(header->mtime    , "");
        strcpy(header->chksum   , "");
        strcpy(&header->typeflag , "");
        strcpy(header->linkname , "");
        strcpy(header->magic    , "");
        strcpy(header->version  , "");
        strcpy(header->uname    , "");
        strcpy(header->gname    , ""); 
        strcpy(header->devmajor , "");// no fuzzing required
        strcpy(header->devminor , "");// no fuzzing required
        strcpy(header->prefix   , "");// no fuzzing required
        strcpy(header->padding  , "");// no fuzzing required
*/

/**
 * @brief fuzz name by:
 * - testing every non ASCII character at position 0 in the name
 * - testing a non ASCII character at every position in the name    
 * @param executable of the tar extractor 
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found 
 *          1 if a erroneous archive has been found
 */
int fuzz_name(char* executable)
{
    printf("===== fuzz name \n");
    
    // archive creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test every non ASCII character at position 0 in the name
    for(int i =128; i < 256; i++)
    {
        char c = (char) i;

        // Fill in the header
        header->name[0] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);
        
        // Write header and file into archive
        if( tar_write("archive.tar", header, content) == -1) 
        {
            ERROR("Unable to write the tar file");
            free(header);
            return -1;
        }

        int rv;
        if( (rv = launches(executable)) == -1 ) 
        {
            ERROR("Error in launches");
            free(header);
            return -1;
        }
        else if (rv == 1) 
        // *** The program has crashed ***
        {
            printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
            return 1;
        }
    }
    

    // Test a non ASCII character at every position in the name
    for(int pos = 0; pos < 100; pos++)
    {
        char c = (char) 128; // first non ASCII character chosen

        // Fill in the header
        header->name[pos] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);
        
        // Write header and file into archive
        if( tar_write("archive.tar", header, content) == -1) 
        {
            ERROR("Unable to write the tar file");
            free(header);
            return -1;
        }

        int rv;
        if( (rv = launches(executable)) == -1 ) 
        {
            ERROR("Error in launches");
            free(header);
            return -1;
        }
        else if (rv == 1) 
        // *** The program has crashed ***
        {
            printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
            return 1;
        }
    }
    
    free(header);
    
    return 0;
}

/**
 * @brief fuzz mode by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * - testing every number at every position
 * @param executable of the tar extractor 
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found 
 *          1 if a erroneous archive has been found
 */
int fuzz_mode(char* executable)
{
    printf("===== fuzz mode \n");
    
    // archive creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test every ascii and non ascii character at position 0
    for( int i = 0; i < 255; i++)
    {
        char c = (char) i;
        // Fill in the header
        strcpy(header->name      , "mode");
        header->mode[0] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);
        
        // Write header and file into archive
        if( tar_write("archive.tar", header, content) == -1) 
        {
            ERROR("Unable to write the tar file");
            free(header);
            return -1;
        }

        int rv;
        if( (rv = launches(executable)) == -1 ) 
        {
            ERROR("Error in launches");
            free(header);
            return -1;
        }
        else if (rv == 1) 
        // *** The program has crashed ***
        {
            printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
            return 1;
        }
    }
    

    // Test a non ascii character at every position
    for( int pos = 0; pos < 8; pos++)
    {
        char c = (char) 128; // first non ascii character chosen
        
        // Fill in the header
        strcpy(header->name      , "mode");
        header->mode[pos] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);
        
        // Write header and file into archive
        if( tar_write("archive.tar", header, content) == -1) 
        {
            ERROR("Unable to write the tar file");
            free(header);
            return -1;
        }

        int rv;
        if( (rv = launches(executable)) == -1 ) 
        {
            ERROR("Error in launches");
            free(header);
            return -1;
        }
        else if (rv == 1) 
        // *** The program has crashed ***
        {
            printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
            return 1;
        }
    }
    
    // Test every number at every position
    for(int pos = 0; pos < 8; pos ++)
    {
        for(int i = 0; i< 10; i++)
        {
            char c = (char) i; 
        
            // Fill in the header
            strcpy(header->name      , "mode");
            header->mode[pos] = c;
            char* content = "Hello World !";
            strcpy(header->size      , "013");
            strcpy(&header->typeflag , "g");     // g = Global extended header
            strcpy(header->magic     , "ustar"); // TMAGIC = ustar
            strcpy(header->version   , "00");
            calculate_checksum(header);
            
            // Write header and file into archive
            if( tar_write("archive.tar", header, content) == -1) 
            {
                ERROR("Unable to write the tar file");
                free(header);
                return -1;
            }

            int rv;
            if( (rv = launches(executable)) == -1 ) 
            {
                ERROR("Error in launches");
                free(header);
                return -1;
            }
            else if (rv == 1) 
            // *** The program has crashed ***
            {
                printf("--- AN ERRONEPIS ARCHIVE FOUND \n");
                return 1;
            }
        }
    }
    free(header);
    
    return 0;
}





/**
 * @brief 
 * @param executable of the tar extractor 
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found 
 *          1 if a erroneous archive has been found
 */
int fuzz_size(char* executable)
{
    printf("===== fuzz size \n");
        
    // archive creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test all octal value at all position
    for(int pos = 0; pos < 11; pos++)
    {
        for(int i = 0; i< 8; i++)
        {
            char c = '0' + i;

            // Fill in the header
            strcpy(header->name, "size");
            char* content = "Hello World !";
            header->size[pos] = c;
            strcpy(&header->typeflag , "g");     // g = Global extended header
            strcpy(header->magic     , "ustar"); // TMAGIC = ustar
            strcpy(header->version   , "00");
            calculate_checksum(header);
            
            // Write header and file into archive
            if( tar_write("archive.tar", header, content) == -1) 
            {
                ERROR("Unable to write the tar file");
                free(header);
                return -1;
            }

            int rv;
            if( (rv = launches(executable)) == -1 ) 
            {
                ERROR("Error in launches");
                free(header);
                return -1;
            }
            else if (rv == 1) 
            // *** The program has crashed ***
            {
                printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
                return 1;
            }
        }
    }
    
    free(header);

    return 0;
}

// =============================================
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        ERROR("Not enough args");
        return -1;
    }
    
    int crashed = 0; // count the number of program that crashed
    int rslt; 
    
    // =============== FUZZ name of the file ==================
    if( (rslt = fuzz_name(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ mode of the file ==================
    if( (rslt = fuzz_mode(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ size of the file ==================
    if( (rslt = fuzz_size(argv[1])) != -1)
    {
        crashed += rslt;
    }

    printf("%d programs crashed \n", crashed);
    return 0;
}