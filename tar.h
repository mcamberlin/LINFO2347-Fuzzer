#ifndef __TAR__
#define __TAR__

struct tar_t
{                              /* byte offset */
    char name[100];               /*   0 */ // Merlin OK
    char mode[8];                 /* 100 */ // Merlin OK
    char uid[8];                  /* 108 */ // Zoé
    char gid[8];                  /* 116 */ // Zoé 
    char size[12];                /* 124 */ // Merlin OK
    char mtime[12];               /* 136 */ // Merlin OK
    char chksum[8];               /* 148 */ // Zoé
    char typeflag;                /* 156 */ // Zoé
    char linkname[100];           /* 157 */ // Merlin OK
    char magic[6];                /* 257 */ // Merlin OK
    char version[2];              /* 263 */ // Zoé
    char uname[32];               /* 265 */ // Zoé
    char gname[32];               /* 297 */ // Merlin OK
    char devmajor[8];             /* 329 */ // no fuzzing required
    char devminor[8];             /* 337 */ // no fuzzing required
    char prefix[155];             /* 345 */ // no fuzzing required
    char padding[12];             /* 500 */ // no fuzzing required
};


int tar_write(const char* tar_name, const struct tar_t* header, const char* file);

#endif