#ifndef __TAR__
#define __TAR__

struct tar_t
{                              /* byte offset */
    char name[100];               /*   0 */ // Merlin
    char mode[8];                 /* 100 */ // Merlin
    char uid[8];                  /* 108 */ // Zoé
    char gid[8];                  /* 116 */ // Zoé
    char size[12];                /* 124 */ // Merlin
    char mtime[12];               /* 136 */ // Merlin
    char chksum[8];               /* 148 */
    char typeflag;                /* 156 */ // Zoé
    char linkname[100];           /* 157 */
    char magic[6];                /* 257 */
    char version[2];              /* 263 */
    char uname[32];               /* 265 */
    char gname[32];               /* 297 */
    char devmajor[8];             /* 329 */
    char devminor[8];             /* 337 */
    char prefix[155];             /* 345 */
    char padding[12];             /* 500 */
};


int tar_write(const char* tar_name, const struct tar_t* header, const char* file);

#endif