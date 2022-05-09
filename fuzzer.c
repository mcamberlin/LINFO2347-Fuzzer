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

    // Test every ascii and non ascii character at position 0
    for( int i = 0; i < 255; i++)
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


    // Test a non ascii character at every position
    for( int pos = 0; pos < 100; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

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
 * @brief fuzz uid by:
 * - 
 * - 
 * - 
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_uid(char* executable)
{
    printf("===== fuzz uid \n");

    // archive creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }
    // Test all characters from ASCII table and extended ASCII table in the name: https://ascii-tables.com/
    for(int i =0; i <255; i++)
    {
        char c = (char) i;

        // Fill in the header
        strcpy(header->name     , "uid");
        header->uid[0] = c;
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
            ERROR("FOUND AN ARCHIVE THAT CRASHED");
            return 1;
        }
    }
    free(header);
    return 0;
}


/**
 * @brief fuzz gid by:
 * - 
 * - 
 * - 
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */


/**
 * @brief fuzz size by:
 * - testing all octal value at all position
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
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
    for(int pos = 0; pos < 12; pos++)
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

    // Test every ascii and non ascii character at position 0
    for( int i = 0; i < 255; i++)
    {
        char c = (char) i;
        // Fill in the header
        strcpy(header->name      , "size");
        char* content = "Hello World !";
        header->size[0] = c;
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
    for( int pos = 0; pos < 12; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "size");
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

    free(header);

    return 0;
}


/**
 * @brief fuzz mtime by:
 * - testing all octal value at all position
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_mtime(char* executable)
{
    printf("===== fuzz mtime \n");

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
            strcpy(header->name, "mtime");
            char* content = "Hello World !";
            strcpy(header->size, "013");
            header->mtime[pos] = c;
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

    printf("==== HERE 2 \n");
    strcpy(header->mtime, "00000000000");

    // Test every ascii and non ascii character at position 0
    for( int i = 0; i < 255; i++)
    {
        char c = '0' +(char) i;
        // Fill in the header
        strcpy(header->name      , "mtime");
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        header->mtime[0] = c;
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

    printf("==== HERE 3 \n");
    // Test a non ascii character at every position
    for( int pos = 0; pos < 11; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "mtime");
        char* content = "Hello World !";
        strcpy(header->size, "013");
        header->mtime[pos] = c;
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
 * @brief fuzz chksum by:
 * -
 * -
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */


/** 
 * @brief fuzz typeflag by:
 * - testing all ASCII character into typeflag
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_typeflag(char* executable)
{
    printf("===== fuzz typeflag \n");

    // archive creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }
    // Test all characters from ASCII table and extended ASCII table in the typeflag
    for(int i =0; i <255; i++)
    {
        char c = (char) i;

        // Fill in the header
        strcpy(header->name     , "typeflag");
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        header->typeflag = c;
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
 * @brief fuzz linkname by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * - testing with linkname with a non ascii and without content into the archive
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_linkname(char* executable)
{
    printf("===== fuzz linkname \n");

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
        strcpy(header->name      , "linkname");
        char* content = "Hello World !";
        strcpy(header->size, "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        header->linkname[0] = c;
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
    for( int pos = 0; pos < 100; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "linkname");
        char* content = "Hello World !";
        strcpy(header->size, "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        header->linkname[pos] = c;
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


    // Test a non ascii character at every position without content
    for( int pos = 0; pos < 100; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "linkname");
        strcpy(&header->typeflag , "2");     // g = Global extended header
        header->linkname[pos] = c;
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);

        // Write header and file into archive
        if( tar_write("archive.tar", header, NULL) == -1)
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
 * @brief fuzz magic by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_magic(char* executable)
{
    printf("===== fuzz magic \n");

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
        strcpy(header->name      , "magic");
        char* content = "Hello World !";
        strcpy(header->size, "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        header->magic[0] = c;
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
    for( int pos = 1; pos < 5; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "magic");
        char* content = "Hello World !";
        strcpy(header->size, "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        header->magic[pos] = c;
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
 * @brief fuzz version by:
 * - 
 * - 
 * - 
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */

/**
 * @brief fuzz uname by:
 * - 
 * - 
 * - 
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */


/**
 * @brief fuzz gname by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_gname(char* executable)
{
    printf("===== fuzz gname \n");

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
        strcpy(header->name      , "gname");
        char* content = "Hello World !";
        strcpy(header->size, "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        header->gname[0] = c;
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
    for( int pos = 0; pos < 32; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "gname");
        char* content = "Hello World !";
        strcpy(header->size      , "013");
        strcpy(&header->typeflag , "g");     // g = Global extended header
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        header->gname[pos] = c;
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
/*
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

    // =============== FUZZ uid of the file ==================
    if( (rslt = fuzz_uid(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ gid of the file ==================


    // =============== FUZZ size of the file ==================
    if( (rslt = fuzz_size(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ mtime of the file ==================
    if( (rslt = fuzz_mtime(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ chksum of the file ==================


    // =============== FUZZ typeflag of the file ==================
    if( (rslt = fuzz_typeflag(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ linkname of the file ==================
    if( (rslt = fuzz_linkname(argv[1])) != -1)
    {
        crashed += rslt;
    }
    
    // =============== FUZZ magic of the file ==================
    if( (rslt = fuzz_magic(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ version of the file ==================


    // =============== FUZZ gname of the file ==================
    if( (rslt = fuzz_gname(argv[1])) != -1)
    {
        crashed += rslt;
    }
*/

    printf("%d programs crashed \n", crashed);
    return 0;
}
