/*
 *  unblackedges.c
 *  Isabelle Lai(ilai01), Max Anavian(manavi01)
 *  2/2/2020
 *  Homework 2 iii
 *  Purpose: Reads in a given pbm format file and turns all black edge pixels 
             into white pixels, removing any black border from the image. Then
             prints altered pixels to standard out in P1 pbm format.
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include "bit2.h"
#include "seq.h"
#include "assert.h"

/* Black and White Constants */
const int BLACK = 1;
const int WHITE = 0;

/* struct to hold the location of a bit in the 2D bitmap */
struct bit_location {
    int col;
    int row;
};

/* function prototypes */
Bit2_T populate_bit_map(Pnmrdr_T rdr, Bit2_T bit_map);
void bit_path(Bit2_T bit_map, Seq_T queue, int col, int row);
void queue_pixel(Bit2_T bit_map, Seq_T queue, int col, int row);
void traverse_bit_map(Bit2_T bit_map);
void print_bit_map(Bit2_T bit_map);

/* main to run program */
int main(int argc, char * argv[]) 
{
     Pnmrdr_T rdr = NULL;
     Bit2_T bit_map = NULL;
     (void) bit_map;
     (void) rdr;
     assert(argc <= 2);
     /* case when user provides image through stdin */
     if (argc == 1) {
         rdr = Pnmrdr_new(stdin);
         bit_map = populate_bit_map(rdr, bit_map);
     } 
     /* case when user provides image file in command line */
     else if (argc == 2) {
         FILE *fp = fopen(argv[1], "r");
         /* check to make sure that file is not null */
         assert(fp != NULL);
         /* read the file with pnmrdr interface */
         rdr = Pnmrdr_new(fp);
         bit_map = populate_bit_map(rdr, bit_map);
         fclose(fp);
     }
     traverse_bit_map(bit_map); 
     print_bit_map(bit_map);
     Bit2_free(&bit_map);
     Pnmrdr_free(&rdr);
     exit(EXIT_SUCCESS);
}

/* Function : populate_bit_map
 * Arguments: a Pnmrdr reader already opened to read in the pbm image, and the
              uninitialized bit map
 * Returns  : the bit map, populated with all the pixel data from the 
              the pbm image
 * Details  : creates the bit map and stores the pixel data from the image
 * Effects  : updates the bit map passed in and returns it
 */
Bit2_T populate_bit_map(Pnmrdr_T rdr, Bit2_T bit_map)
{
    (void) bit_map;
     /* check to make sure that reader is not null */
     assert(rdr != NULL);
     /* reads pixel data from the reader */
     Pnmrdr_mapdata data = Pnmrdr_data(rdr);
     /* check to make sure the pbm data type is correct */
     assert(data.type == Pnmrdr_bit);
     bit_map = Bit2_new(data.width, data.height);
     /* check to make sure that the size of image is not zero */
     int pbm_size = data.width * data.height;
     assert(pbm_size != 0);
     
     /* loop through all pixels in the pbm file to populate the 
      * Uarray */
      for (unsigned j = 0; j < data.height; j++) {
          for (unsigned i = 0; i < data.width; i++) {
              int bit = Pnmrdr_get(rdr);
              Bit2_put(bit_map, i, j, bit);
          }  
      }
     return bit_map;
}
    
/* Function : bit_path
 * Arguments: a bit_map created in main, a Queue, and a int col and row
 * Returns  : N/A
 * Details  : Traverses through the bits and finds a path. It checks to see if 
              a pixel is black. If it is, it turnes that pixel white, then 
              adds the neighboring black pixels into the queue. Once 
              all the neighboring pixels are added, they are dequeued and then 
              run through the same algorithm
 * Effects  : Calls queue_pixels, which adds the neighboring black pixels into 
              the queue to be eventually popped off. Each if statement refers
              to the 4 possible neighboring pixels it could be.
 */
void bit_path(Bit2_T bit_map, Seq_T queue, int col, int row)
{
    assert(bit_map != NULL);
    assert(queue != NULL);
    /* Queue locations of the black pixels */
    if (Bit2_get(bit_map, col, row) == BLACK) {
        Bit2_put(bit_map, col, row, WHITE);
        struct bit_location *location = malloc(sizeof(*location));
        assert(location != NULL);
        location -> col = col;
        location -> row = row;
        Seq_addhi(queue, location);
        
        while (Seq_length(queue) != 0) {
            struct bit_location *temp = Seq_remlo(queue);
            /* check left neighboring pixel */
            if ((temp -> col != 0) && 
                (Bit2_get(bit_map, (temp -> col) - 1, temp -> row) == BLACK)) {
                queue_pixel(bit_map, queue, (temp -> col) - 1, temp -> row);
            }
            /* check right neighboring pixel */
            if ((temp -> col != Bit2_width(bit_map) - 1) && 
                (Bit2_get(bit_map, (temp -> col) + 1, temp -> row) == BLACK)) {
                queue_pixel(bit_map, queue, (temp -> col) + 1, temp -> row);
            }       
            /* check above neighboring pixel */
            if ((temp -> row != 0) && 
                (Bit2_get(bit_map, temp -> col, (temp -> row) - 1) == BLACK)) {
                queue_pixel(bit_map, queue, temp -> col, (temp -> row) - 1);
            }
            /* check below neighboring pixel */
            if ((temp -> row != Bit2_height(bit_map) - 1) && 
                (Bit2_get(bit_map, temp -> col, (temp -> row) + 1) == BLACK)) {
                queue_pixel(bit_map, queue, temp -> col, (temp -> row) + 1);
            }
            assert(temp != NULL);
            free(temp);
        }
    }
}

/* Function : queue_pixel
 * Arguments: bit map holding the pixel data, queue storing the pixels to be
              checked and turned white, and the col and row of the current 
              pixel, which should be a black edge pixel
 * Returns  : N/A
 * Details  : changes a black edge pixel to be white and adds it to the queue 
              to so its neighboring pixels can be checked
 * Effects  : adds the pixel to the passed in queue and changes the 
              bit stored at the given col and row to be white
 */
void queue_pixel(Bit2_T bit_map, Seq_T queue, int col, int row)
{
    assert(bit_map != NULL);
    assert (queue != NULL);
    /* adding new white pixel to bit_map */
    struct bit_location *temp_location = malloc(sizeof(*temp_location));
    assert(temp_location != NULL);
    temp_location -> col = col;
    temp_location -> row = row;
    Seq_addhi(queue, temp_location);
    Bit2_put(bit_map, temp_location -> col, temp_location -> row, WHITE);
}

/* Function : traverse_bit_map
 * Arguments: a bit_map created in main
 * Returns  : N/A
 * Details  : It creates the queue and each call to bit path calls one edge. 
              It goes through the top and bottom most "true" edges and then
              the left and right most "true" sides. It frees the sequence 
              once all the pixels have been dequeued.
 * Effects  : Calls bit_map on each pixel on the sides of the file. This then 
              removes the correct black edges and adds them to the queue 
              created in this function. Also frees memory of queue once done
 */
void traverse_bit_map(Bit2_T bit_map) 
{
    assert(bit_map != NULL);
    /* Creating new sequence & traversing the whole map */
    Seq_T queue = Seq_new(100);
    
    /* tracersing cols */
    for (int i = 0; i < Bit2_height(bit_map); i++) {
        bit_path(bit_map, queue, 0, i);
        bit_path(bit_map, queue, Bit2_width(bit_map) - 1, i);
    }
    
    /* traversing rows */
    for (int i = 0; i < Bit2_width(bit_map); i++) {
        bit_path(bit_map, queue, i, 0);
        bit_path(bit_map, queue, i, Bit2_height(bit_map) - 1);
    }
    Seq_free(&queue);
}

/* Function : print_bit_map
 * Arguments: the bit map, altered to have only white borders
 * Returns  : N/A
 * Details  : prints the altered bit map into a P1 format pbm
 * Effects  : prints file header and pixels to standard output
 */
void print_bit_map(Bit2_T bit_map) 
{
    assert(bit_map != NULL);
    /* print plain portable bitmap format */
    printf("P1\n%d %d\n", Bit2_width(bit_map), Bit2_height(bit_map));
    int line_counter = 1;
    for (int j = 0; j < Bit2_height(bit_map); j++) {
        for (int i = 0; i < Bit2_width(bit_map); i++) {
            int bit = Bit2_get(bit_map, i, j);
            printf("%i", bit);
            line_counter++;
             /* every 70 lines a new line is printed */
            if (line_counter >= 70) {
                printf("\n");
                line_counter = 1;
            }
        }
    }
}

