/*
 *  simlines.c
 *  Kyle Sayers, Max Anavian
 *  1/31/2020
 *  Homework 1
 *  Notes: Checks for similar lines in files. Uses a data structure
 *         consisting of a Hanson table that holds keys and values.
 *         Keys are atoms to lines, values are Hanson lists that
 *         hold struct Answers that contain info about line name and
 *         line number.
 *  NEEDS WORK: This program sometimes segfaults in seemingly random places
                (the location in which it seg faults changes very frequently).
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "readaline.h"
#include "table.h"
#include "list.h"
#include "assert.h"
#include "mem.h"
#include "list.h"
#include "atom.h"

/* struct Answer, which pertains to each line */
struct Answer {
    char *file_name;
    int line_num;
};


/*
 *  Function : add_to_table
 *  Arguments : Hanson Table, file line, the lineNumer, file name
 *  Returns : None
 *  Details : Creates a new struct to hold data. Creates an Atom as 
 *  the hanson key. Creates a new list, or adds to the current list
 *  of structs of each line key
 */ 
void add_to_table(Table_T **table, char *file_line, 
                  int lineNumber, char *file_name_str) {
    /* To hold the line and the atom */
    const char *line = file_line;
    const char *atomHolder;

    /* Prepares mallocated answer ptr */
    struct Answer *answer_ptr = malloc(sizeof(struct Answer));
    if (answer_ptr == NULL) {
        fprintf(stderr, "%s\n", "Could not allocate memory");
        return;
    }
    answer_ptr->file_name = file_name_str;
    answer_ptr->line_num = lineNumber;

    /* creating atom */
    atomHolder = Atom_string(line);

    /* Searches the Hanson Table for the key */
    List_T result = Table_get(**table, atomHolder);

    if (result == NULL) {
        /* Creates new List and adds to key */
        List_T answers_list = List_list(answer_ptr, NULL);
        Table_put(**table, atomHolder, answers_list);

    } else {
        /* updates existing list of structs */
        List_T answers_list = List_push(result, answer_ptr);
        Table_put(**table, atomHolder, answers_list);
        result = Table_get(**table, atomHolder);
    }
}

/*
 * Function : Is Word Character
 * Arguments: Character in question
 * Returns  : Bool true or false
 * Details  : Determines whether character
              is word-char as defined by specs
 */
bool is_word_char(char c) {
    return ( (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
             (c >= '0' && c <= '9') ||
             (c == '-') );
}

/*
 * Function : Line Cleaner
 * Arguments: (uncleaned) line_p, (cleaned) cleaned_pp
 * Returns  : None
 * Details  : Removes non-word-chars from string 
              and allocates memory for cleaned string
 */
void line_cleaner(char *line_p, char **cleaned_pp) {
    if (line_p == NULL) {
        fprintf(stderr, "%s\n", "Line uninitialized");
        return;
    }

    char cleaned_line[strlen(line_p)];

    /* i = iterator for line_p */
    /* j = iterator for cleaned_line */
    int i = 0;
    int j = 0;
    int first_word_placed = 0;
    while (line_p[i] != '\n' && line_p[i] != '\0') {
        /* If character in question is word-char */
        if (is_word_char(line_p[i])) {
            /* If it's not the first char,
               and previous character is non-word-char */
            if ((i != 0) && !is_word_char(line_p[i - 1]) 
                && (first_word_placed == 1)) {
                    cleaned_line[j] = ' ';
                    j++;
            }

            /* Add character to cleaned line */
            cleaned_line[j] = line_p[i];
            first_word_placed = 1;
            j++;
        }
        i++;
    }
    /* Add end line and end of string char */
    cleaned_line[j] = '\n';
    cleaned_line[j + 1] = '\0';

    /* Allocate memory */
    char *allocated_mem = (char *) malloc((strlen(cleaned_line)) 
                                           * sizeof(char) + 1);
    if (allocated_mem == NULL) {
        fprintf(stderr, "%s\n", "Could not allocate memory");
        return;
    }

    /* Put cleaned line data into allocated space */
    strcpy(allocated_mem, cleaned_line);

    /* Pass cleaned line data into cleaned_pp */
    *cleaned_pp = allocated_mem;
}

/*
 * Function  : Print Results
 * Arguments : Hanson table, Hanson list of lines read
 * Returns   : None
 * Details   : Prints results in the proper format
 * NEEDS WORK: Currently frees some data, when it's ideally done
 *             in another function.
 */
void print_results(Table_T **table, List_T *lines_read) {
    void *x;
    char *line;
    struct Answer *answer;

    if (lines_read == NULL) {
        return;
    }

    /* Continuously pops lines_read */
    while (List_length(*lines_read) > 0) {
        *lines_read = List_pop(*lines_read, &x);
        line = x;

        /* creating atom */
        const char *atomHolder;
        atomHolder = Atom_string(line);

        /* Searches the Hanson Table for the key */
        List_T answers_list = Table_get(**table, atomHolder);

        /* If there's less than 2 entries, don't print anything */
        if (List_length(answers_list) < 2) {
            continue;
        }

        /* Reverse the list, due to the order in which entries are entered */
        answers_list = List_reverse(answers_list);
        
        printf("%s", line);
        /* Continuously pop answers */
        while (List_length(answers_list) > 0) {
            answers_list = List_pop(answers_list, &x);
            answer = x;

            /* Print formatted with left justified */
            printf("%-20s", answer->file_name);
            printf("%d\n", answer->line_num);

            /* Free answer */
            free(answer);
        }
        free(line);
        printf("\n");
    }
}

/*
 * Function  : Free Data
 * Arguments : Hanson table, (copy of) Hanson list of lines read
 * Returns   : None
 * NEEDS WORK: This function is not currently used.
 *             It takes a copy of the lines_read list (that
 *             hasn't already been popped by print_results).
 *             We had difficulties properly freeing data from our
 *             data structure.
 */
void free_data(Table_T **table, List_T *lines_read) {
    void *x;
    char *line;
    struct Answer *answer;

    if (lines_read == NULL) {
        return;
    }

    /* Continuously pops lines_read */
    while (List_length(*lines_read) > 0) {
        *lines_read = List_pop(*lines_read, &x);
        line = x;

        /* creating atom */
        const char *atomHolder;
        atomHolder = Atom_string(line);

        /* Searches the Hanson Table for the key */
        List_T answers_list = Table_get(**table, atomHolder);
        
        while (List_length(answers_list) > 0) {
            answers_list = List_pop(answers_list, &x);
            answer = x;
            free(answer);
        }
        free(line);
        List_free(&answers_list);
    }
    Table_free(*table);
}

/*
 * Function : Add to lines_read List
 * Arguments: Hanson table, Hanson list lines_read, string line
 * Returns  : None
 * Details  : Adds a line to the lines_read list if it hasn't already been
            : added to the table (this is to avoid duplicate lines)
 */
void add_to_lines_read_list(Table_T **table, List_T *lines_read, char *line) {

    /* creating atom */
    const char *atomHolder;
    atomHolder = Atom_string(line);

    /* Searches the Hanson Table for the key */
    List_T *result = Table_get(**table, atomHolder);

    /* If it's not already in the table, and by extension the list, push */
    if (result == NULL) {
        *lines_read = List_push(*lines_read, line);
    }
}

/*
 * Function : Prepare Results
 * Arguments: argc, argv, Hanson table, Hanson list lines_read, string line
 * Returns  : None
 * Details  : Prepares results by reading each file line by line and inserting
              cleaned lines into the table and chronicling each distinct line
              in lines_read
 */
void prepare_results(int argc, char **argv, Table_T *lines_table_ptr, 
                     List_T *lines_read) {
    /* Prepare variables */
    FILE* file;
    char *line = "";
    char *cleaned = NULL;

    /* Read in, clean, lines and put in table */
    for (int i = 1; i < argc; i++) {
        char *file_name = argv[i];

        file = fopen(file_name, "r");

        /* Checks to see if file was opened */
        if (file == NULL) {
            fprintf(stderr, "%s\n", "Cannot open file");
            exit(EXIT_FAILURE);
        }

        /* Continuously get lines from file until end */
        int line_num_iterator = 0;
        readaline(file, &line);
        while (line != NULL) {
            line_cleaner(line, &cleaned);

            /* Check if line is just \n */
            if (strcmp(cleaned, "\n") == 0) {
                readaline(file, &line);
                line_num_iterator++;
                continue;
            }

            /* Adds to lines_read */
            add_to_lines_read_list(&lines_table_ptr, lines_read, cleaned);

            /* Add to table */
            add_to_table(&lines_table_ptr, cleaned,
                         line_num_iterator, file_name);

            free(line);
            readaline(file, &line);
            line_num_iterator++;
        }
        /* Close file once done */
        fclose(file);
    }
}

int main(int argc, char **argv) {
    /* Prepare table */
    Table_T lines_table = Table_new(200, NULL, NULL);
    Table_T *lines_table_ptr = &lines_table;

    /* Prepare lines_read list */
    List_T lines_read = NULL;

    /* Prepare results */
    prepare_results(argc, argv, lines_table_ptr, &lines_read);

    /* Print results using function */
    /*List_T lines_read_copy = List_copy(lines_read);*/
    print_results(&lines_table_ptr, &lines_read);

    /* 
     * NEEDSWORK: Unsure of how to properly free memory stored 
     * in data structure. All freeing is currently done in 
     * print_results
     */
    /*List_free(&lines_read);*/
    /*free_data(&lines_table_ptr, &lines_read_copy);*/
    Table_free(&lines_table);
    exit(EXIT_SUCCESS);
}
