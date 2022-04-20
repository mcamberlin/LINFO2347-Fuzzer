#include <stdio.h> // pour fprintf()
#include <errno.h> // pour le detail des erreurs
#include <stdio.h>
#include <string.h>

#include "tar.h"


/**
 * Launches another axecutable given as argument,
 * parses its output and check whether or not it matches "*** The program has crashed ***".
 * @param the path to the executable
 * @return -1 if the executable cannot be launched,
 *          0 if it is launched but does not print "*** The program has crashed ***",
 *          1 if it is launched and prints "*** The program has crashed ***".
 */
int launcher(int argc, char* argv[])
{
    if (argc < 2)
        return -1;
    int rv = 0;
    char cmd[51];
    strncpy(cmd, argv[1], 25);
    cmd[26] = '\0';
    strncat(cmd, " archive.tar", 25);
    char buf[33];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if(fgets(buf, 33, fp) == NULL) {
        printf("No output\n");
        goto finally;
    }
    if(strncmp(buf, "*** The program has crashed ***\n", 33)) {
        printf("Not the crash message\n");
        goto finally;
    } else {
        printf("Crash message\n");
        rv = 1;
        goto finally;
    }
    finally:
    if(pclose(fp) == -1) {
        printf("Command not found\n");
        rv = -1;
    }
    return rv;
}

/**
 * Computes the checksum for a tar header and encode it on the header
 * @param entry: The tar header
 * @return the value of the checksum
 */
unsigned int calculate_checksum(struct tar_t* entry){
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

/**
 * fuzz the name field of a header with: - all non ascii character for name[0]
 *                                       - a non ascii character for all index of name[index]
 *                                       - all number and all upper and lower letters of name[0]
 * @param count_crash: pointer that counts the number of crash cases
 * @param count_other_msg : pointer that counts the non crashes message
 * @param argument : the path to the executable
 */
void fuzz_name(int *count_crash, int *count_other_msg, char* argument)
{
    int flag=0;
    struct tar_t* header;
    if( ( header = (struct tar_t*) calloc(1,sizeof(struct tar_t))) == NULL)
    {
        error("Unable to calloc header in fuzz_name \n");
    }

    unsigned int first;
    for(first= 0x80; first!=0xFF && flag!=1; first++)
    {
        header->name[0] = first;
        strcpy(header->magic, "ustar");
        strcpy(header->version, "00");
        header->typeflag = 'g';
        char content[6]="AAAAA";
        char size_of_content = (char) sizeof(content);
        strcpy(header->size, "05");
        int check = calculate_checksum(header);
        int ret = tar_write(header, content); 
        flag = check_extractor(count_crash, count_other_msg, argument, header->name);
    }

    free(header);    
}

// =============================================
int main(int argc, char* argv[])
{

    struct tar_t* header;


    tar_write(header, "test.txt");
    

    // Generate input files

    // Check wether the converter crashes
}