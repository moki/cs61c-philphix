#ifndef _PHILPHIX_H
#define _PHILPHIX_H

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFFER_SIZE 1

extern uint32_t stringHash(void *string);

extern int stringEquals(void *s1, void *s2);

extern char **readDictionary(char *dictName, uint32_t *kvsc);

extern void processInput(hash_table_t *dictionary);

#endif
