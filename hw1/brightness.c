/*
 *  Brightness.c
 *  Kyle Sayers, Max Anavian
 *  1/31/2020
 *  Homework 1
 *  Notes: Calculates the brightness of a pgm file
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "pnmrdr.h"
#include "except.h"

/* Function : Get Average Brightness
 * Arguments: Image map pointer, a denominator, a image size
 * Returns  : The average brightness of the file
 * Details  : Gets each brightness value, scales it with the denominator and 
 *            then calculates the average brightness of the image 
 */

float getAvBrightness(void * image_map, 
                      float image_denominator, 
                      float image_size) {
    float brightness_sum = 0;
    float temp_brightness = 0;

    /*calculate the average brightness, reading in for the entirety of file*/
    for (int i = 0; i < image_size; i ++) {    
        temp_brightness = Pnmrdr_get(image_map);
        brightness_sum += temp_brightness / image_denominator;
    }

    /*return brightness casted as a float*/
    return (float) brightness_sum / image_size;
}

int main(int argc, char **argv) {

    float av_brightness = 1.0;

    FILE* file_p;

    /*pointer to hold our image map from pnmdr*/
    void *image_map = NULL;
    Pnmrdr_mapdata image_data;


    /*depending on arguments, read in file from stdin*/
    if (argc == 1) {
        file_p = stdin;
    } else if (argc == 2) {
        file_p = fopen(argv[1], "r");
    } else if (argc > 2) {
        fprintf(stderr, "Too many arguments!\n");
        exit(EXIT_FAILURE);
    }



    TRY

        image_map = Pnmrdr_new(file_p);
        image_data = Pnmrdr_data(image_map);

        /* Check if file is correct format*/
        if (image_data.type !=  Pnmrdr_gray) { 
            fprintf(stderr, "Wrong file type!\n");
            exit(EXIT_FAILURE);
        } 

        /* If is null, exit */
        if (file_p == NULL) { 
            fprintf(stderr, "File is NULL, Cannot be opened!\n");
            exit(EXIT_FAILURE);
        } 
        
        
    /* Exceptions for bad formatting*/
    EXCEPT(Pnmrdr_Badformat)
        fprintf(stderr, "%s\n", "Bad pnmrdr format");
        exit(EXIT_FAILURE);
        
    /*all other unhandled errors*/
    ELSE

        fprintf(stderr, "%s\n", "Unknown error");
        exit(EXIT_FAILURE);

    END_TRY;
   

    /* holds the scale for which the image brightness is dependent */
    float image_denominator = image_data.denominator;

    /* getting image size to divide # of pixels */
    float image_size = image_data.width * image_data.height;
   

    /* getAvBrightness */
    av_brightness = getAvBrightness(image_map, image_denominator, image_size);

    /* Print result */
    printf("Average is : %0.3f\n", av_brightness);

    /* free the image_map pointer */
    free(image_map);

    /* Closing the file */
    fclose(file_p);
    exit(EXIT_SUCCESS); 
    
}
