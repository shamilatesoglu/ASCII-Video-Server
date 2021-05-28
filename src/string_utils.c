//
// Created by MSA on 28/05/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "string_utils.h"

/*
 * A function fills the buffer with the content of the file
 * with given name in the working directory, on disk.
 */
void
read_file(char *filename, char *buffer)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        char msg[100];
        sprintf(msg, "Error while opening the file %s", filename);
        perror(msg);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET); /* To reset seek. */

    fread(buffer, fsize, 1, f);
    fclose(f);

    buffer[fsize] = '\0';
}

/* Counts number of non-overlapping times given substring mentioned in string. */
int
count_substring(char *str, char *sub)
{
    int count = 0, len = strlen(sub);

    while (*str != '\0')
    {
        if (strncmp(str++, sub, len))
            continue;
        str += len - 1;
        count++;
    }
    return count;
}

/*
 * Splits the given string with supplied delimiter.
 * Returns a pointer to array of strings.
 *
 * Warning!
 * The supplied string must be dynamically allocated.
 * And it will be modified.
 */
char **
split(char *str, char *delimiter)
{
    int i = 0;
    char **array = (char **) malloc((count_substring(str, delimiter) + 1) * sizeof(char *));
    char *p = strtok(str, delimiter);

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, delimiter);
    }

    return array;
}

/*
 * Removes trailing and leading whitespace characters of the given string.
 * This function modifies the given string, reallocates and returns a new pointer
 * to that string.
 */
char *
trim(char *str)
{
    /* For leading whitespaces. */
    int i;
    while (str[0] == ' ')
    {
        int length = strlen(str);
        for (i = 0; i < length; i++)
        {
            str[i] = str[i + 1];
        }
        str = STR_REALLOC(str, strlen(str));
    }

    /* For trailing whitespaces. */
    while (str[strlen(str) - 1] == ' ')
    {
        str[strlen(str) - 1] = '\0';
        str = STR_REALLOC(str, strlen(str));
    }

    return str;
}
