/*
 *  bit2.c
 *  Isabelle Lai(ilai01), Max Anavian(manavi01) 
 *  2/2/2020
 *  Homework 2 iii
 *  Purpose : The implementation of a 2D Bit Uarray using Hansons Uarray 
              inteface. It allows users to make new, free, get width, height 
              and size, as well as get a specific location and traverse row 
              and col majors.
 */ 
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include "bit2.h"
 #include "bit.h"
 #include "assert.h"

 #define T Bit2_T

/* Function : Bit2_new
 * Arguments: an int width and height of the new bit set, which should be 
              greater than zero
 * Returns  : The new bit set
 * Details  : Allocates space for and creates a new bit set of the 
 *            given width and height using Hanson's bit interface
              and stores its width and height
 * Effects  : N/A
 */
T Bit2_new(int width, int height) 
{
    T bitset = malloc(sizeof(*bitset));
    assert(bitset != NULL);
    bitset -> set = Bit_new(width * height);
    bitset -> width = width;
    bitset -> height = height;
    return bitset;
}


/* Function : Bit2_free
 * Arguments: a pointer to a bit set, which should not be NULL
 * Returns  : N/A
 * Details  : Frees all the memory associated with the bit set
 * Effects  : N/A           
 */
void Bit2_free(T *bitset) 
{
    assert(bitset != NULL);
    assert(&((*bitset) -> set) != NULL);
    Bit_free(&((*bitset) -> set));
    free(*bitset);
}

/* Function : Bit2_width
 * Arguments: a bit set, which should not be NULL
 * Returns  : the int width
 * Details  : returns the width of the bit set
 * Effects  : N/A
 */
int Bit2_width(T bitset)
{
    return bitset -> width;
}

/* Function : Bit2_height
 * Arguments: a bit set, which should not be NULL
 * Returns  : the int height
 * Details  : returns the height of the bit set
 * Effects  : N/A
 */
int Bit2_height(T bitset) 
{
    return bitset -> height;
}

/* Function : Bit2_put
 * Arguments: a bit set, which should not be NULL,
              int col and int row of the element in the bitset to be set, which
              should be greater than zero, and the int value of the bit to set
 * Returns  : the previous bit's value at the given location
 * Details  : Uses Hanson's bit interface to find the previous bit's 
 *            value and replace it with the given bit value
 * Effects  : N/A
 */
int Bit2_put(T bitset, int col, int row, int bit) 
{
    int prev_bit = Bit_get(bitset -> set, (col * bitset -> height) + row);
    Bit_put(bitset -> set, (col * bitset -> height) + row, bit);
    return prev_bit;
}

/* Function : Bit2_get
 * Arguments: a bit set, which should not be NULL,
              and int col and int row of the bit to get, which should be 
              greater than zero
 * Returns  : the int value of the bit at the given location
 * Details  : Uses Hanson's bit interface to find the current bit's 
 *            value at the current col and row.
 * Effects  : N/A
 */
int Bit2_get(T bitset, int col, int row) 
{
    return Bit_get(bitset -> set, (col * bitset -> height) + row);
}
    
/* Function : Bit2_map_row_major
 * Arguments: a bit set which should not be NULL,
              *function apply(int, int, void *, void *) which itself takes
              the int col and row of an element in the bit set, the bit set,
              the int value of a bit, and a *void closure,
              and a *bool
 * Returns  : N/A
 * Details  : Calls a given apply function on every bit in the set traversing 
              row by row
 * Effects  : Calls the function pointed to by *apply
 */
void Bit2_map_row_major(T bitset, 
                        void (*apply)(int, int, T, int, void *), 
                        bool *OK) 
{
                            
    for(int i = 0; i < bitset -> height; i++) {
        for(int j = 0; j < bitset -> width; j++) {
            int bit = Bit2_get(bitset, j, i);
            apply(j, i, bitset, bit, OK);
        }
    }                        
}

/* Function : Bit2_map_col_major
 * Arguments: a bit set which should not be NULL,
              *function apply(int, int, void *, void *) which itself takes
              the int col and row of an element in the bit set, the bit set,
              the int value of a bit, and a *void closure,
              and a *bool
 * Returns  : N/A
 * Details  : Calls a given apply function on every bit in the set traversing 
              col by col
 * Effects  : Calls the function pointed to by *apply
 */
void Bit2_map_col_major(T bitset,
                        void (*apply)(int, int, T, int, void *), 
                        bool *OK) 
{
    for (int i = 0; i < bitset -> width; i++) {
        for (int j = 0; j < bitset -> height; j++) {
            int bit = Bit2_get(bitset, i, j);
            apply(i, j, bitset, bit, OK);
        }
    }
}




