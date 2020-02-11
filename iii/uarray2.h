/*
 *  uarray2.h
 *  Isabelle Lai(ilai01), Max Anavian(manavi01)
 *  2/2/2020
 *  Homework 2 iii
 *  Purpose: The .h file containing the interface for our UArray2 
             implementation. It defines the functions we write in uarray2.c
 */ 

#include <stdbool.h>

#define T UArray2_T
typedef struct T *T;

/* creates new UArray2 */
T UArray2_new(int width, int height, int size);

/* frees all memory associated with UArray2 */
void UArray2_free(T *uarray2);

/* returns width of UArray2 */
int UArray2_width(T uarray2);

/* returns height of UArray2 */
int UArray2_height(T uarray2);

/* returns size of an element in the UArray2 */
int UArray2_size(T uarray2);

/* returns a pointer to the element at the given col and row */
void *UArray2_at(T uarray2, int col, int row);

/* traverses the UArray2 row by row and calls function apply on each element */
void UArray2_map_row_major(T uarray2, 
                           void (*apply)(int, int, T, void *, void *), 
                           bool *OK);
                           
/* traverses the UArray2 col by col and calls function apply on each element */
void UArray2_map_col_major(T uarray2, 
                           void (*apply)(int, int, T, void *, void *), 
                           bool *OK);

#undef T

















