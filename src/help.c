/**
 * @file help.c
 * @author Merlin Camberlin (0944-1700), Zoé Schoofs (3502-1700)
 * @brief This file contains helpers functions used in fuzzer.c but that does not really belong to the fuzzing part of the project.
 * @version 0.1
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <string.h>

#include "tar.h"

int success_nb = 0;

#define ERROR(descr, ...) fprintf(stderr, "Error: " descr "\n", ##__VA_ARGS__);
/** 
 * Launches another executable given as argument,
 * parses its output and check whether or not it matches "*** The program has crashed ***".
 * @param the path to the extractor
 * @return -1 if the executable cannot be launched,
 *          0 if it is launched but does not print "*** The program has crashed ***",
 *          1 if it is launched and prints "*** The program has crashed ***".
 */
int launches(char* executable)
{
    int rv = 0;
    char cmd[51];
    strncpy(cmd, executable, 25); // copy executable into cmd up to 25 Bytes
    cmd[26] = '\0'; // add the null Byte

    strncat(cmd, " archive.tar", 25); // append archive.tar to cmd

    char buf[33]; // output buffer: size 33 because 33 chars in "*** The program has crashed ***\n"
    
    FILE *fp;
    if ((fp = popen(cmd, "r")) == NULL) 
    {
        ERROR("Error opening pipe!");
        return -1;
    }
   
    /*
    if(fgets(buf, 33, fp) == NULL) 
    {
        ERROR("No output");
        goto finally;
    }
    */
   fgets(buf, 33, fp);
    

    // Program has crashed
    if(strncmp(buf, "*** The program has crashed ***\n", 33) == 0) 
    {
        printf("Crash message\n");
        rv = 1;
        success_nb = success_nb + 1;
                
        // rename archive.tar by success_#number.tar
        char new_name [16];
        strcpy(new_name, "success_#");
        char str[6];
        str[0] = success_nb + '0';
        str[1] = '.';
        str[2] = 't';
        str[3] = 'a';
        str[4] = 'r';

        strcat(new_name, str);
        int ret; 
        if( (ret = rename("archive.tar", new_name)) !=0) 
        {
            ERROR("Error archive.tar renaming");
        }
        goto finally;
        
    } 
    // Program has NOT crashed
    else 
    {
        printf("Not the crash message\n");
        goto finally;
    }
    
    finally:
    if(pclose(fp) == -1) 
    {
        ERROR("Command not found");
        rv = -1;
    }
    return rv;
}

/**
 * Computes the checksum for a tar header and encode it on the header
 * @param entry: The tar header
 * @return the value of the checksum
 */
unsigned int calculate_checksum(struct tar_t* entry)
{
    // use spaces for the checksum bytes while calculating the checksum
    memset(entry->chksum, ' ', 8);

    // sum of entire metadata
    unsigned int check = 0;
    unsigned char* raw = (unsigned char*) entry;
    for(int i = 0; i < 512; i++){
        check += raw[i];
    }

    snprintf(entry->chksum, sizeof(entry->chksum), "%06o0", check);

    entry->chksum[6] = '\0';
    entry->chksum[7] = ' ';
    return check;
}