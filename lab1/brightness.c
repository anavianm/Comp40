/*
 *  Brightness.c
 *  Kyle Sayers, Max Anavian
 *  1/24/2020
 *  Lab 1
 *  Notes: Calculates the brightness of a pgm file
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Function : Skip Comments
 * Arguments: File_p (already opened)
 * Returns  : None
 * Details  : Skips pgm magic number and # comment lines
 */
void skipComments(FILE *file_p) {
    
    char c;

    /* Skip magic number */
    while ((c = fgetc(file_p)) != EOF) {
        if (c == '\n') {
            c = fgetc(file_p);
            break;
        }
    }

    /* Skip comments */
    while (c == '#') {
        while ((c = fgetc(file_p)) != EOF) {
            if (c == '\n') {
                c = fgetc(file_p);
                break;
            }
        }
    }

    fseek(file_p, -1, SEEK_CUR);
}

/* Function : Read Header
 * Arguments: File_p (already opened), width_p, height_p, max brightness_p
 * Returns  : Success(1) or failure(0) bool
 * Details  : Gets basic data from pgm file header 
 * and returns it via pointers to arguments
 */
int readHeader(FILE *file_p,
               int *file_width, 
               int *file_height,
               int *max_brightness) {
    char c;

    /* Get height */
    fscanf(file_p, "%d", file_height);

    /* Ensure there's a space inbetween values */
    if ((c = fgetc(file_p)) != ' ') {
        return 0;
    }

    /* Get width */
    fscanf(file_p, "%d", file_width);

    /* Ensure there's a \n inbetween values */
    while ((c = fgetc(file_p)) != EOF) {
        if (c == '\n') {
            break;
        }
        continue;
    }

    /* Get max_brightness */
    fscanf(file_p, "%d", max_brightness);

    /* Ensure there's a \n after max_brightness */
    while ((c = fgetc(file_p)) != EOF) {
        if (c == '\n') {
            break;
        }
        continue;
    }

    fseek(file_p, -1, SEEK_CUR);

    return 1;
}

/* Function : Get Average Brightness
 * Arguments: File_p (already opened)
 * Returns  : The average brightness of the file
 * Details  : Gets each decimal value and averages them
 */
float getAvBrightness(FILE* file_p) {
    int brightness_sum = 0;
    int total_pixels = 0;
    int temp_brightness;
    char c;

    while ((c = fgetc(file_p)) != EOF) { 
        if (fscanf(file_p, "%d", &temp_brightness) == 1) {
            brightness_sum += temp_brightness;
            total_pixels++;
        }
    }

    return (float) brightness_sum/total_pixels;
}

int main(int argc, char **argv) {

    (void) argc;

    char filename[100];

    int file_width = -1;
    int file_height = -1;
    int max_brightness = -1;
    float av_brightness = -1.0;
    FILE* file_p;


    

    if (argc == 1) {
        scanf("%s", filename);
    } else if (argc == 2) {
        strcpy(filename, argv[1]);
    } else if (argc > 2) {
        fprintf(stderr, "Too many arguments!\n");
        exit(EXIT_FAILURE);
    }

    /* Open file */
    file_p = fopen(filename, "r");
    if (file_p == NULL) { 
        fprintf(stderr, "Cannot open file!\n");
        exit(EXIT_FAILURE);
    } 

    /* Skip comments */
    skipComments(file_p);

    /* Read headers */
    if (!readHeader(file_p, &file_width, &file_height, &max_brightness)) {
        exit(EXIT_FAILURE);
    }

    /* getAvBrightness */
    av_brightness = getAvBrightness(file_p);

    /* Print result */
    printf("Average is : %f\n", av_brightness);

    fclose(file_p); 

    exit(EXIT_SUCCESS); 
}
