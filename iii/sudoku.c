/*
 *  sudoku.c
 *  Isabelle Lai(ilai01), Max Anavian(manavi01)
 *  2/2/2020
 *  Homework 2 iii
 *  Purpose: Reads in a sudoku board as a graymap and then checks the 
 *           correctness of the board. It traverses through the board 
             and makes sure that no number is repeated and the board is
             "correct". It asserts that the board is 9x9. otherwise it exits 
             with 1
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include "uarray2.h"
#include "assert.h"

const int SUDOKUSIZE = 9;
 
/* function prototypes */
UArray2_T populate_board(Pnmrdr_T rdr, UArray2_T sudoku_board);
void check_and_print(int i, int j, UArray2_T a, void *p1, void *p2);
void check_sudoku_correctness(UArray2_T sudoku_board);
void check_num_set(int i, 
                  int j, 
                  UArray2_T sudoku_board, 
                  void *place_holder,
                  void *num_checker);
void check_square(int row_start, 
                  int col_start,
                  UArray2_T sudoku_board, 
                  int *num_checker);

/* main to run program */
int main(int argc, char * argv[]) 
{
     Pnmrdr_T rdr = NULL;
     UArray2_T sudoku_board = NULL;
     assert(argc <= 2);
     /* case when user provides image through stdin */
     if (argc == 1) {
         rdr = Pnmrdr_new(stdin);
         sudoku_board = populate_board(rdr, sudoku_board);
     } 
     /* case when user provides pgm image file through command line*/
     else if (argc == 2) {
         FILE *fp = fopen(argv[1], "r");
         /* check to make sure that file is not null */
         assert(fp != NULL);
         /* read the file with Pnmrdr interface */
         rdr = Pnmrdr_new(fp);
         sudoku_board = populate_board(rdr, sudoku_board);
         fclose(fp);
     }
     check_sudoku_correctness(sudoku_board);
     UArray2_free(&sudoku_board);
     Pnmrdr_free(&rdr);
     exit(EXIT_SUCCESS);
     
}

/* Function : populate_board
 * Arguments: a Pnmrdr reader already opened to read in the pgm image and the
              uninitialized UArray2 sudoku board
 * Returns  : the sudoku board, populated with all the sudoku data from the 
              the pgm image
 * Details  : creates the sudoku board and stores the sudoku data from the 
              image into the board
 * Effects  : updates the sudoku board UArray passed in and returns it
 */
UArray2_T populate_board(Pnmrdr_T rdr, UArray2_T sudoku_board) 
{
     /* check to make sure that reader is not null */
     assert(rdr != NULL);
     /* reads pixel data from the reader */
     Pnmrdr_mapdata data = Pnmrdr_data(rdr);

     /* check to make sure the pgm data type is correct */
     assert(data.type == Pnmrdr_gray);
     
     sudoku_board = UArray2_new(data.width, data.height, sizeof(unsigned));

     /* check that the size of image is not zero */
     int pgm_size = data.width * data.height;
     assert(pgm_size != 0);
     
     /* check that sudoku board is nine by nine */
     assert(data.width == (unsigned)SUDOKUSIZE);
     assert(data.height == (unsigned)SUDOKUSIZE);
     
     /* loop through all pixels in the pgm file to populate the 
      * Uarray */
     for (unsigned i = 0; i < data.width; i++) {
         for (unsigned j = 0; j < data.height; j++) {
             unsigned numerator = Pnmrdr_get(rdr);
             /* check that values are within correct range for sudoku */
             assert(numerator >= 1);
             assert(numerator <= (unsigned)SUDOKUSIZE);
             unsigned *pixel = UArray2_at(sudoku_board, i, j);
             *pixel = numerator;

         }
     }
     return sudoku_board;
}
 
/* Function : check_sudoku_correctness
 * Arguments: A sudoku board of type UArray2_T 
 * Returns  : N/A
 * Details  : Sets the numChecker array all to 0 and calls
              the get square function on each 3 by 3 square 
              Calls row and col major and checks each row and col
 * Effects  : This function checks the correctness for the entire board. It 
              goes through each 3 by 3 square first and then checks the rows 
              and col of the sudoku board
 */
void check_sudoku_correctness(UArray2_T sudoku_board)
{
    assert(sudoku_board != NULL);
    int arr_size = SUDOKUSIZE + 1;
    int num_checker[arr_size];
    for (int i = 0; i < arr_size; i++) {
        num_checker[i] = 0;
    }
    
    /* checks mini 3x3 squares */
    for (int i = 0; i < SUDOKUSIZE; i += 3) {
        for (int j = 0; j < SUDOKUSIZE; j += 3) {
            check_square(i, j, sudoku_board, num_checker);
        }
    }
    
    /* checks col and row correctness */
    UArray2_map_row_major(sudoku_board, check_num_set, (bool *) num_checker);
    UArray2_map_col_major(sudoku_board, check_num_set, (bool *) num_checker);
}

/* Function : check_num_set
 * Arguments: the col and row of the element on the board to check, the sudoku
              board, a *void place_holder that allows the use of the UArray2
              interface map major functions, and a pointer to an int array that
              stores which numbers have already been seen in the col or row
 * Returns  : N/A
 * Details  : checks that a given row or col of the sudoku board has only 
              numbers 1 thru 9, with no repeats
 * Effects  : exits the program with code of 1 if the sudoku solution was not 
              valid, otherwise updates the given int array at the index of the 
              number on the sudoku board at [col, row] to be 1
 */
void check_num_set(int i, 
                   int j, 
                   UArray2_T sudoku_board, 
                   void *place_holder, 
                   void *num_checker)
{
    (void) place_holder;
    assert(sudoku_board != NULL);
    unsigned *value = UArray2_at(sudoku_board, i, j);
    
    /* exits if number has already been found in current row or col */
    if ((((int*)num_checker)[(int)*(value) - 1] == 1) 
           && (((int*)num_checker)[SUDOKUSIZE] < SUDOKUSIZE - 1)){
        exit(1);
    } else {
    /* otherwise set number as "seen" and sets others to 0 */
        ((int*)num_checker)[(int)*(value) - 1] = 1;
        if (((int*)num_checker)[SUDOKUSIZE] < SUDOKUSIZE - 1){
            ((int*)num_checker)[SUDOKUSIZE] += 1;
        } else {
            /* if entire row or col has been checked and with no repeats, 
               resets num_checker array to all zeros */
            for (int i = 0; i < SUDOKUSIZE + 1; i++) {
                ((int *)num_checker)[i] = 0;
            }
        }
    }
}

/* Function : check_square
 * Arguments: the row and col of the board to start checking at, the sudoku
              board, and an *int array to track which numbers have already 
              been seen within the square
 * Returns  : N/A
 * Details  : Calls check_num_set on each 3x3 set of squares in the board 
 * Effects  : calls check_num_set, which exits the program with code of 1 if 
              the sudoku solution is incorrect, otherwise updates num_checker
              with value of 1
 */
void check_square(int row_start, 
                  int col_start, 
                  UArray2_T sudoku_board, 
                  int *num_checker) 
{
    assert(sudoku_board != NULL);
    for (int i = row_start; i < row_start + 3; i++) {
        for (int j = col_start; j < col_start + 3; j++) {
            check_num_set(i, j, sudoku_board, NULL, num_checker);
        }
    }
}
