/*
 *  uarray2.c
 *  Isabelle Lai, Max Anavian 2/2/2020
 *  Homework 2 iii
 *  Purpose : The implementation of a 2D Uarray using Hansons Uarray inteface. 
              It allows users to make new, free, get width, height and size, 
              as well as get a specific location and traverse row and col
              majors.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray2.h"
#include "uarray.h"
#include "assert.h"

#define T UArray2_T
    
/* Function : UArray2_new
 * Arguments: the int width and height of the new UArray2 and the size of each
              element to be stored in the UArray2, all which should be
              greater than zero
 * Returns  : a new UArray2
 * Details  : Allocates space for and creates a new UArray2 set of the 
 *            given width and height using Hanson's UArray interface
 * Effects  : N/A
 */
T UArray2_new(int width, int height, int size)
{
    T uarray2 = (T) UArray_new(width, size * height);
    assert(uarray2 != NULL);
    for(int i = 0; i < width; i ++) {
        UArray_T *col = UArray_at((UArray_T) uarray2, i);
        *(col) = UArray_new(height, size);
    }
    return uarray2;
    
}

/* Function : UArray2_free
 * Arguments: a pointer to a UArray2 set, which should not be NULL
 * Returns  : N/A
 * Details  : Frees all the memory associated with UArray2
 * Effects  : N/A        
 */
void UArray2_free(T *uarray2)
{
    assert(uarray2 != NULL);
    for(int i = 0; i < UArray_length((UArray_T) *uarray2); i ++) {
        UArray_T *col = UArray_at((UArray_T) *uarray2, i);
        UArray_free(col);
    }
    UArray_free((UArray_T *) uarray2);
}


/* Function : UArray2_width
 * Arguments: a UArray2, which should not be NULL
 * Returns  : the int width of the UArray2
 * Details  : Uses Hanson's UArray interface to find the width of the 
 *            UArray2
 * Effects  : N/A
 */
int UArray2_width(T uarray2)
{
    return UArray_length((UArray_T) uarray2);
}

/* Function : UArray2_height
 * Arguments: a UArray2
 * Returns  : the int height
 * Details  : Uses Hanson's UArray interface to find the height of the 
 *            UArray2
 */
int UArray2_height(T uarray2)
{
    UArray_T *col = UArray_at((UArray_T) uarray2, 1);
    return UArray_length(*col);
    
}

/* Function : UArray2_size
 * Arguments: a UArray2, which should not be NULL
 * Returns  : the int size of the element type held by the UArray2
 * Details  : Uses Hanson's UArray interface to find the size of the 
 *            UArray2
 * Effects  : N/A
 */
int UArray2_size(T uarray2)
{
    return UArray_size((UArray_T) uarray2) / UArray2_height(uarray2);
}

/* Function : UArray2_at
 * Arguments: a UArray2, int col, int row
 * Returns  : A void pointer to the element at current location
 * Details  : Uses Hanson's UArray interface to find the Current
 *            element at col row
 * Effects  : N/A
 */
void *UArray2_at(T uarray2, int col, int row)
{
    UArray_T *column = UArray_at((UArray_T) uarray2, col);
    return UArray_at(*column, row);
}

/* Function : UArray2_map_row_major
 * Arguments: a UArray2 which should not be NULL,
              *function apply(int, int, void *, void *) which itself takes
              the int col and row of an element in the UArray2, the UArray2,
              and two *void,
              and a *bool
 * Returns  : N/A
 * Details  : Calls a given apply function on every UArray2 element in the set 
 *            traversing row by row
 * Effects  : Calls the function pointed to by *apply
 */
void UArray2_map_row_major(T uarray2, 
                           void (*apply)(int, int, T, void *, void *), 
                           bool *OK)
{

    for (int i = 0; i < UArray2_height(uarray2); i++) {
        for (int j = 0; j < UArray2_width(uarray2); j++) {
            void *p1 = UArray2_at(uarray2, j, i);
            apply(j, i, uarray2, p1, OK);
            
        }
    }
}

/* Function : UArray2_map_col_major
 * Arguments: a UArray2 which should not be NULL,
              *function apply(int, int, void *, void *) which itself takes
              the int col and row of an element in the UArray2, the UArray2,
              and two *void,
              and a *bool//  
 * Returns  : N/A
 * Details  : Calls a given apply function on every UArray2 element in the set 
             traversing col by col
 * Effects  : Calls the function pointed to by *apply
 */
void UArray2_map_col_major(T uarray2, 
                           void (*apply)(int, int, T, void *, void *), 
                           bool *OK)
{
                                            
    for (int i = 0; i < UArray2_width(uarray2); i++) {
        for (int j = 0; j < UArray2_height(uarray2); j++) {
            void *p1 = UArray2_at(uarray2, i, j);
            apply(i, j , uarray2, p1, OK);
        }
    }
}




