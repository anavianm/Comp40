/*
 *  readaline.c
 *  Kyle Sayers, Max Anavian 1/22/2020
 *  Homework 1
 *  Reads one line from a given opened file. Has a max line length of 200.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int MAX_LINE_LEN = 200;

/*
 *  Function : readaline
 *  Arguments : a file and pointer to the line
 *  Returns : size
 *  Details : Allocates memory for **datapp. Reads in file and returns 
 *  the size of the line.
 */ 
size_t readaline(FILE *inputfd, char **datapp) {
    char c;
    char *line_p;

    /* Allocate memory for line */
    line_p = (char *) malloc(MAX_LINE_LEN * sizeof(char));
    if (line_p == NULL) {
        fprintf(stderr, "%s\n", "Could not allocate memory");
    }
    /* Fills line with \0s */
    memset (line_p, '\0', sizeof(char) * MAX_LINE_LEN);

    /* Get first character */
    c = fgetc(inputfd);

    /* Empty file base case */
    if (c == EOF) {
        *datapp = NULL;
        return 0;
    }

    /* Read line */
    int i = 0;
    while (c != EOF) {

        /* If it reaches maximum */
        if (i >= MAX_LINE_LEN) {
            printf("%s%d%s\n", "Line more than ", 
                   MAX_LINE_LEN, 
                   " characters. Line cannot be read");

            while (c != EOF) {
                c = fgetc(inputfd);
                if (c == '\n') {
                    break;
                }
            }

            free(line_p);
            *datapp = "\n";
            return sizeof(*datapp);
        }

        /* Go until end of line */
        if (c == '\n') {
            line_p[i] = '\n';
            break;
        }

        line_p[i] = c; 
        c = fgetc(inputfd);
        i++;
    }

    /* Pass result */
    *datapp = line_p;
    return sizeof(*datapp);
}
