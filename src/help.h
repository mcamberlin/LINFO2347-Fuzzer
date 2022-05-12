/**
 * @file help.h
 * @author Merlin Camberlin (0944-1700), Zoé Schoofs (3502-1700)
 * @brief This signature of the helper functions used in the fuzzer.c file
 * @version 0.1
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __HELP__
#define __HELP__


int launches(char* executable);

unsigned int calculate_checksum(struct tar_t* entry);

#endif