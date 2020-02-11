/*
 *  bit2.h
 *  Isabelle Lai (ilai01), Max Anavian (manavi01) 
 *  2/2/2020
 *  Homework 2 iii
 *  Purpose: The .h file containing the interface for our Bit2
             implementation. It defines the functions we write in bit2.c
 */ 

#include "bit.h"

#define T Bit2_T
typedef struct T *T; 

/* struct to store the Bit2 itself and its width and height */
struct T {
    Bit_T set;
    int width;
    int height;
};

/* creates new Bit2 */
T Bit2_new(int width, int height);

/* frees all memory associated with Bit2 */
void Bit2_free(T *bitset);

/* returns width of Bit2 */
int Bit2_width(T bitset);

/* returns height of Bit2 */
int Bit2_height(T bitset);

/* sets the bit at the given col and row to the given value */
int Bit2_put(T bitset, int col, int row, int bit);

/* gets the bit value stored at the given col and row */
int Bit2_get(T bitset, int col, int row);

/* traverses the Bit2 row by row and calls function apply on each element */
void Bit2_map_row_major(T bitset, 
                        void (*apply)(int, int, T, int, void *), 
                        bool *OK);

/* traverses the Bit2 col by col and calls function apply on each element */
void Bit2_map_col_major(T bitset,
                        void (*apply)(int, int, T, int, void *), 
                        bool *OK);

#undef T

















