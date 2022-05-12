/**
 * @file fuzzer.c
 * @author Merlin Camberlin (0944-1700), Zoé Schoofs (3502-1700)
 * @brief This file contains multiple functions which purpose is to fuzz the generation of an tar archive.
 * @version 0.1
 * @date 2022-05-13
 * @tool To clean the folder run this command: rm !(Makefile|extractor|*.tar) 
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h> // for printf, fprintf
#include <stdlib.h> // for malloc, calloc, free
#include <string.h> // for strncpy, memset, strlen

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

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test every ascii and non ascii character at position 0
    for( int i = 1; i < 256; i++)
    {
        char c = (char) i;
        // Fill in the header
        header->name[0] = c;
        strcpy(header->mode, "07777");
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
    for( int pos = 0; pos < 99; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        header->name[pos] = c;
        strcpy(header->mode, "07777");
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test every ascii and non ascii character at position 0
    for( int i = 0; i < 256; i++)
    {
        char c = (char) i;
        // Fill in the header
        strcpy(header->name      , "mode");
        header->mode[0] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
        strcpy(header->size      , "015");
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
            char c = (char) i+48;

            // Fill in the header
            strcpy(header->name      , "mode");
            header->mode[pos] = c;
            char* content = "Hello World !";

            strcpy(header->size      , "015");

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


/**
 * @brief fuzz uid by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * - testing every number at every position
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_uid(char* executable)
{
    printf("===== fuzz uid \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test a non ascii character at every position
    for( int pos = 0; pos < 8; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "uid");
        strcpy(header->mode     , "07777");
        header->uid[pos] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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

    
    // Test all characters from ASCII table and extended ASCII table in the name: https://ascii-tables.com/
    for(int i =0; i <256; i++)
    {
        char c = (char) i;

        // Fill in the header
        strcpy(header->name     , "uid");
        strcpy(header->mode     , "07777");
        header->uid[0] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
    

    // Test every number at every position
    for(int pos = 0; pos < 8; pos ++)
    {
        for(int i = 0; i< 10; i++)
        {
            char c = (char) i;

            // Fill in the header
            strcpy(header->name      , "uid");
            strcpy(header->mode     , "07777");
            header->uid[pos] = c;
            char* content = "Hello World !";
            strcpy(header->size      , "015");

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


/**
 * @brief fuzz gid by:
 * - all octal values in all positions (because gid need to be an octal or raise an error)
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
 int fuzz_gid(char* executable)
 {
   printf("===== fuzz gid \n");

   // header creation
   struct tar_t* header;
   if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
   {
       ERROR("Unable to malloc header");
       return -1;
   }

   // Test every number at every position
   for(int pos = 0; pos < 8; pos ++)
   {
       for(int i = 0; i< 8; i++)
       {
           char c = (char) i +48;

           // Fill in the header
           strcpy(header->name      , "gid");
           strcpy(header->mode, "07777");
           header->gid[pos] = c;
           char* content = "Hello World !";
           strcpy(header->size      , "015");
           strcpy(header->magic, "ustar");
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

    // header creation
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
            char c = i + 48; // convert int to char

            // Fill in the header
            strcpy(header->name      , "size");
            strcpy(header->mode      , "07777");
            char* content = "Hello World !";
            header->size[pos] = c;

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
    for( int i = 0; i < 256; i++)
    {
        char c = (char) i;
        // Fill in the header
        strcpy(header->name      , "size");
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        header->size[0] = c;

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
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        header->size[pos] = c;

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

    // header creation
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
            char c = i + 48; // convert int to corresponding char

            // Fill in the header
            strcpy(header->name, "mtime");
            strcpy(header->mode, "07777");
            char* content = "Hello World !";
            strcpy(header->size, "015");
            header->mtime[pos] = c;
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
    for( int i = 0; i < 256; i++)
    {
        char c = (char) i;
        // Fill in the header
        strcpy(header->name      , "mtime");
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size      , "015");
        header->mtime[0] = c;
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
        strcpy(header->name      , "mtime");
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size, "015");
        header->mtime[pos] = c;

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
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * - testing with linkname with a non ascii and without content into the archive
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_chksum(char* executable)
{
    printf("===== fuzz chksum \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test all characters from ASCII table and extended ASCII table in the name: https://ascii-tables.com/
    for(int i =0; i <256; i++)
    {
        char c = (char) i;

        // Fill in the header
        strcpy(header->name     , "checksum");
        strcpy(header->mode     , "07777");
        header->chksum[0] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
    // Test a non ascii character at every position
    for( int pos = 0; pos < 8; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "checksum");
        strcpy(header->mode     , "07777");
        header->chksum[pos] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
            char c = (char) i+48;

            // Fill in the header
            strcpy(header->name      , "checksum");
            strcpy(header->mode     , "07777");
            header->chksum[pos] = c;
            char* content = "Hello World !";
            strcpy(header->size      , "015");

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

    // header creation
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
        strcpy(header->mode     , "07777");
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
 * - testing with linkname with a non ascii and without content into the archive
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_linkname(char* executable)
{
    printf("===== fuzz linkname \n");

    // header creation
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
        strcpy(header->mode      , "07777");
        char* content =  "Hello World !";
        strcpy(header->size      , "015");
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

    // Test a non ascii character at every position without content
    for( int pos = 0; pos < 99; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "linkname");
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size, "015");
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

    // header creation
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
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size, "015");
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
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * - testing every number at every position
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
 int fuzz_version(char* executable)
 {
     printf("===== fuzz version \n");

     // header creation
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
         strcpy(header->name     , "version");
         strcpy(header->mode     , "07777");
         header->version[0] = c;
         char* content = "Hello World !";
         strcpy(header->size      , "015");
         strcpy(header->magic     , "ustar"); // TMAGIC = ustar
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
     // Test a non ascii character at every position
     for( int pos = 0; pos < 8; pos++)
     {
         char c = (char) 128; // first non ascii character chosen

         // Fill in the header
         strcpy(header->name      , "version");
         strcpy(header->mode     , "07777");
         header->version[pos] = c;
         char* content = "Hello World !";
         strcpy(header->size      , "015");
         strcpy(header->magic     , "ustar"); // TMAGIC = ustar
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

     // Test every number at every position
     for(int i = 0; i<10; i++)
     {
       char c = (char) i +48;
       strcpy(header->name      , "version");
       strcpy(header->mode     , "07777");
       header->version[0] = c;

       for(int j =0; j<10;j++){
         char d = (char) j+48;
         // Fill in the header
         header->version[1] = d;
         char* content = "Hello World !";
         strcpy(header->size      , "015");
         strcpy(header->magic     , "ustar"); // TMAGIC = ustar
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

/**
 * @brief fuzz uname by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character at every position
 * - testing every number at every position
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_uname(char* executable)
{
    printf("===== fuzz uname \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test every ascii and non ascii character at position 0
    for(int i =0; i <255; i++)
    {
        char c = (char) i;

        // Fill in the header
        strcpy(header->name     , "uname");
        strcpy(header->mode     , "07777");
        header->uname[0] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
    
    // Test a non ascii character at every position
    for( int pos = 0; pos < 32; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "uname");
        strcpy(header->mode     , "07777");
        header->uname[pos] = c;
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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
    for(int pos = 0; pos < 32; pos ++)
    {
        for(int i = 0; i< 9; i++)
        {
            char c = (char) i+48;

            // Fill in the header
            strcpy(header->name      , "uname");
            strcpy(header->mode     , "07777");
            header->uname[pos] = c;
            char* content = "Hello World !";
            strcpy(header->size      , "013");

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

    // header creation
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
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size, "015");

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
    for( int pos = 0; pos < 31; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "gname");
        strcpy(header->mode      , "07777");
        char* content = "Hello World !";
        strcpy(header->size      , "015");
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


/**
 * @brief fuzz no end of archive by:
 * - creating archive without end-of-archive marker (2x 512-byte zero bytes blocks)
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_no_end_of_archive(char* executable)
{
    printf("===== fuzz end of archive \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Fill in the header
    strcpy(header->name      , "end_of_archive");
    strcpy(header->mode      , "07777");
    char* content = "Hello World !";
    strcpy(header->size      , "015");
    strcpy(header->magic     , "ustar"); // TMAGIC = ustar
    strcpy(header->version   , "00");
    calculate_checksum(header);

    // Write header and file into archive
    if( tar_write_without_end_of_archive("archive.tar", header, content) == -1)
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

    free(header);

    return 0;
}

/**
 * @brief fuzz no padding by:
 * - creating an archive without padding at the end of the file
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_no_padding(char* executable)
{
    printf("===== fuzz no padding \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Fill in the header
    strcpy(header->name      , "no_padding");
    strcpy(header->mode      , "07777");
    char* content = "Hello World !";
    strcpy(header->size      , "015");
    strcpy(header->magic     , "ustar"); // TMAGIC = ustar
    strcpy(header->version   , "00");
    calculate_checksum(header);

    // Write header and file into archive
    if( tar_write_without_padding("archive.tar", header, content) == -1)
    {
        ERROR("Unable to write the tar file without padding");
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

    free(header);

    return 0;
}

/**
 * @brief fuzz data content by:
 * - testing every ascii and non ascii character at position 0
 * - testing a non ascii character in the content at every position until 999th
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_data_content(char* executable)
{
    printf("===== fuzz data content \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Test every ascii and non ascii character at position 0
    for( int i = 1; i < 256; i++)
    {
        char c = (char) i;
        // Fill in the header
        strcpy(header->name      , "data_content");
        strcpy(header->mode      , "07777");
        char content[2] = {c, '\0'};
        sprintf(header->size, "%lo", strlen(content));
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

    // Test a non ascii character in the content at every position until 999th
    for( int pos = 2; pos < 999; pos++)
    {
        char c = (char) 128; // first non ascii character chosen

        // Fill in the header
        strcpy(header->name      , "data_content");
        strcpy(header->mode      , "07777");
        char content[pos];
        
        content[pos-2] = c;
        content[pos-1] = '\0';

        sprintf(header->size, "%o", pos);
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
 * @brief fuzz header no data by:
 * - creating archive with header filled like a file would be stored in it but in fact archive does not contain data
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_header_no_data(char* executable)
{
    printf("===== fuzz header no data \n");

    // header creation
    struct tar_t* header;
    if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        ERROR("Unable to malloc header");
        return -1;
    }

    // Fill in the header
    strcpy(header->name      , "header_no_data");
    strcpy(header->mode      , "07777");
    strcpy(header->size      , "00");
    strcpy(header->magic     , "ustar"); // TMAGIC = ustar
    strcpy(header->version   , "00");
    calculate_checksum(header);

    // Write header
    if( tar_write_with_header_without_data("archive.tar", header, NULL) == -1)
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

    free(header);

    return 0;
}

/**
 * @brief fuzz multiple files by:
 * - creating archive with multiple file entries (header + data) 
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_multiple_files(char* executable)
{
     printf("===== fuzz multiple files \n");

    int n = 10; // the number of file entries to put inside the archive

    struct tar_t* header;
    struct tar_t** headers;
    if( (headers = (struct tar_t**) malloc(n *sizeof(struct tar_t*))) == NULL)
    {
        ERROR("Unable to malloc headers");
        return -1;
    }

    char* content = "Hello World !";
    char** contents;
    if( (contents = (char**) malloc(n *sizeof(char*))) == NULL)
    {
        ERROR("Unable to malloc contents");
        return -1;
    }

    for(int i = 0; i< n; i++)
    {
        // header creation
        if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
        {
            ERROR("Unable to malloc header");
            return -1;
        }

        // Fill in the header
        char name[6];
        sprintf(name, "file%d", i);
        strcpy(header->name      , name);
        strcpy(header->mode      , "07777");
        sprintf(header->size, "%lo", strlen(content));
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);

        headers[i] = header;
        contents[i] = content;
    }
    
    // Write headers and contents into archive
    if( tar_write_multiple_files("archive.tar", headers, contents, n) == -1)
    {
        ERROR("Unable to write multiple files into the tar file");
        free(header);
        free(headers);
        free(contents);
        return -1;
    }

    int rv;
    if( (rv = launches(executable)) == -1 )
    {
        ERROR("Error in launches");
        free(header);
        free(headers);
        free(contents);
        return -1;
    }
    else if (rv == 1)
    // *** The program has crashed ***
    {
        printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
        free(header);
        free(headers);
        free(contents);
        return 1;
    }
   
    free(header);
    free(headers);
    free(contents);
    return 0;
}

/**
 * @brief fuzz multiple files without data by:
 * - creating archive with multiple file entries (header only) 
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_multiple_files_without_data(char* executable)
{
     printf("===== fuzz multiple files without data \n");

    int n = 10; // the number of file entries to put inside the archive

    struct tar_t* header;
    struct tar_t** headers;
    if( (headers = (struct tar_t**) malloc(n *sizeof(struct tar_t*))) == NULL)
    {
        ERROR("Unable to malloc headers");
        return -1;
    }
    
    // char* content = "Hello World !";
    
    char** contents;
    if( (contents = (char**) malloc(n *sizeof(char*))) == NULL)
    {
        ERROR("Unable to malloc contents");
        return -1;
    }
    

    for(int i = 0; i< n; i++)
    {
        // header creation
        if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
        {
            ERROR("Unable to malloc header");
            return -1;
        }

        // Fill in the header
        char name[6];
        sprintf(name, "file%d", i);
        strcpy(header->name      , name);
        strcpy(header->mode      , "07777");
        strcpy(header->size      , "00");
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);

        headers[i] = header;
        contents[i] = NULL; // !!!
    }
    
    // Write headers and contents into archive
    if( tar_write_multiple_files("archive.tar", headers, contents, n) == -1)
    {
        ERROR("Unable to write multiple files into the tar file");
        free(header);
        free(headers);
        free(contents);
        return -1;
    }

    int rv;
    if( (rv = launches(executable)) == -1 )
    {
        ERROR("Error in launches");
        free(header);
        free(headers);
        free(contents);
        return -1;
    }
    else if (rv == 1)
    // *** The program has crashed ***
    {
        printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
        free(header);
        free(headers);
        free(contents);
        return 1;
    }
   
    free(header);
    free(headers);
    free(contents);
    return 0;
}

/**
 * @brief fuzz multiple files with multiple end of marker by:
 * - creating archive with multiple file entries (header + data) all ending with an end_of_archive marker.
 * @param executable of the tar extractor
 * @return -1 if an error occured
 *          0 if no erroneous archive has been found
 *          1 if a erroneous archive has been found
 */
int fuzz_multiple_files_multiple_end_of_archives(char* executable)
{
     printf("===== fuzz multiple files \n");

    int n = 3; // the number of file entries to put inside the archive

    struct tar_t* header;
    struct tar_t** headers;
    if( (headers = (struct tar_t**) malloc(n *sizeof(struct tar_t*))) == NULL)
    {
        ERROR("Unable to malloc headers");
        return -1;
    }

    char* content = "Hello World !";
    char** contents;
    if( (contents = (char**) malloc(n *sizeof(char*))) == NULL)
    {
        ERROR("Unable to malloc contents");
        return -1;
    }

    for(int i = 0; i< n; i++)
    {
        // header creation
        if( (header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
        {
            ERROR("Unable to malloc header");
            return -1;
        }

        // Fill in the header
        char name[6];
        sprintf(name, "file%d", i);
        strcpy(header->name      , name);
        strcpy(header->mode      , "07777");
        sprintf(header->size, "%lo", strlen(content));
        strcpy(header->magic     , "ustar"); // TMAGIC = ustar
        strcpy(header->version   , "00");
        calculate_checksum(header);

        headers[i] = header;
        contents[i] = content;
    }
    
    // Write headers and contents into archive
    if( tar_write_multiple_files_multiple_end_of_archives("archive.tar", headers, contents, n) == -1)
    {
        ERROR("Unable to write multiple files with end-of-archive marker at every end into the tar file");
        free(header);
        free(headers);
        free(contents);
        return -1;
    }

    int rv;
    if( (rv = launches(executable)) == -1 )
    {
        ERROR("Error in launches");
        free(header);
        free(headers);
        free(contents);
        return -1;
    }
    else if (rv == 1)
    // *** The program has crashed ***
    {
        printf("--- AN ERRONEOUS ARCHIVE FOUND \n");
        free(header);
        free(headers);
        free(contents);
        return 1;
    }
   
    free(header);
    free(headers);
    free(contents);
    return 0;
}

// ================================================================================
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        ERROR("Not enough args");
        return EXIT_FAILURE;
    }

    int crashed = 0; // count the number of archives that make the extractor crashed
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

    // =============== FUZZ uid of the file ==================
    // lead to crash
    if( (rslt = fuzz_uid(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ gid of the file ==================
    if( (rslt = fuzz_gid(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ size of the file ==================
    // lead to crash
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
    if( (rslt = fuzz_chksum(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ typeflag of the file ==================
    // lead to crash
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
    // lead to crash
    if( (rslt = fuzz_version(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ uname of the file ==================
    if( (rslt = fuzz_uname(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ gname of the file ==================
    if( (rslt = fuzz_gname(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ end of archive ==================
    // lead to crash BUT NOT DETECTED BY INGINIOUS :/ 
    if( (rslt = fuzz_no_end_of_archive(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ no padding ================== 
    if( (rslt = fuzz_no_padding(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ content of data ==================
    // lead to crash BUT NOT DETECTED BY INGINIOUS :/ 
    if( (rslt = fuzz_data_content(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ header no data ==================
    if( (rslt = fuzz_header_no_data(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ multiple files ==================
    if( (rslt = fuzz_multiple_files(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ multiple files without data ==================
    if( (rslt = fuzz_multiple_files_without_data(argv[1])) != -1)
    {
        crashed += rslt;
    }

    // =============== FUZZ multiple files with each file ending with end-of-archive marker ==================
    if( (rslt = fuzz_multiple_files_multiple_end_of_archives(argv[1])) != -1)
    {
        crashed += rslt;
    }

    printf("%d programs crashed \n", crashed);
    return EXIT_SUCCESS;
}
