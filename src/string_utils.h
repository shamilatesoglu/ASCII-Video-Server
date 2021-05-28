//
// Created by MSA on 28/05/2021.
//

#ifndef STREAMING_SERVER_STRING_UTILS_H
#define STREAMING_SERVER_STRING_UTILS_H

#include "lang_utils.h"

/* Constants */
#define NOT_FOUND -1

/* Defining utility macros. */
#define EQUALS(str1, str2) ((strcmp(str1, str2) == 0) ? true : false)
#define STR_REALLOC(str, size) (char *)realloc(str, (size) * sizeof(char))
#define STR_ALLOC(size) (char *)calloc(size, sizeof(char))
#define FILE_REPO_REALLOC(repo, size) (File *)realloc(repo, (size) * sizeof(File))
#define CONTAINS(str, sub) (strstr(str, sub) != NULL)

void
read_file(char *filename, char *buffer);

int
count_substring(char *str, char *sub);

char **
split(char *str, char *delimiter);

char *
trim(char *str);

#endif //STREAMING_SERVER_STRING_UTILS_H
