#include <stdio.h> // for printf, fprintf
#include <stdlib.h> // for malloc, calloc, free
#include <string.h> // for strncpy, memset, strlen
#include <sys/stat.h> // for size of file
#include <sys/types.h> // for size of file



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
 * @brief 
 * @param path to the executable of the tar extractor 
 * @return -1 if an error occured
 *          int: the number of programs that crashed 
 */
int fuzz_name(char* executable)
{
    printf("===== fuzz name \n");
    
    int crashed = 0;

    // archive creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // 1. Test special character from extended ASCII table in the name: https://ascii-tables.com/
    for(int i =128; i < 256; i++)
    {
        char c = (char) i;
        printf("name: %c \n", c);

        // Fill in the header
        strcpy(header->name     , &c);
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
            return -1;
        }

        int rv;
        if( (rv = launches(executable)) == -1 ) 
        {
            ERROR("Error in launches");
            return -1;
        }
        else if (rv == 1) 
        // *** The program has crashed ***
        {
            ERROR("FOUND AN ARCHIVE THAT CRASHED");
            return 1;
        }
        else
        {
            crashed += rv;
        }
    }
    free(header);
    
    return crashed;
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
    
    // =============== FUZZ name of the file ==================
    int rslt = fuzz_name(argv[1]);
    if(rslt != -1)
    {
        crashed += rslt;
    }

    printf("%d programs crashed \n", crashed);
    return 0;
}