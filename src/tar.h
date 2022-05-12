/**
 * @file tar.h
 * @author Merlin Camberlin (0944-1700), Zoé Schoofs (3502-1700)
 * @brief This file contains the structure of the header and also the different signature of functions used in fuzzer.c to generate archive.
 * @version 0.1
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __TAR__
#define __TAR__

struct tar_t
{                              /* byte offset */
    char name[100];               /*   0 */ 
    char mode[8];                 /* 100 */ 
    char uid[8];                  /* 108 */ 
    char gid[8];                  /* 116 */ 
    char size[12];                /* 124 */ 
    char mtime[12];               /* 136 */ 
    char chksum[8];               /* 148 */ 
    char typeflag;                /* 156 */ 
    char linkname[100];           /* 157 */ 
    char magic[6];                /* 257 */ 
    char version[2];              /* 263 */ 
    char uname[32];               /* 265 */ 
    char gname[32];               /* 297 */ 
    char devmajor[8];             /* 329 */ // no fuzzing required
    char devminor[8];             /* 337 */ // no fuzzing required
    char prefix[155];             /* 345 */ // no fuzzing required
    char padding[12];             /* 500 */ // no fuzzing required
};


int tar_write(const char* tar_name, const struct tar_t* header, const char* file);

int tar_write_without_end_of_archive(const char* tar_name, const struct tar_t* header, const char* content);

int tar_write_without_padding(const char* tar_name, const struct tar_t* header, const char* content);

int tar_write_with_header_without_data(const char* tar_name, const struct tar_t* header, const char* content);

int tar_write_multiple_files(const char* tar_name, struct tar_t** headers, char** contents, int n);

int tar_write_multiple_files_multiple_end_of_archives(const char* tar_name, struct tar_t** headers, char** contents, int n);
#endif
