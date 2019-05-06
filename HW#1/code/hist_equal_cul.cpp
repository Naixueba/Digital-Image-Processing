//(1) Wenjun Li//(2) 8372761120//(3) wenjunli@usc.edu//(4) Jan.22,2019

// HW1.1.(c) Histogram Manipulation
// based on transfer function

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int Size = 400;

    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is grayscale or color
    if (argc < 4) {
        BytesPerPixel = 1; // default is grey image
    } else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5) {
            Size = atoi(argv[4]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[Size][Size][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    int Imagedata_int[Size][Size][3];      // grey image only 1 channel
    //unsigned char Imagedata_hist[Size][Size][BytesPerPixel];  // BytesPerPixel=1;
                                                                // run fail when set BPP=3 ???
    // Question: why it's 1 here, but it's 3 in "Size*Size*3" ?
    int pixel_cumu[3][256] = {0};          // store grey-value
    int pixel_num[3][256] = {0};           // store pixel number

    int i, j;    // length = i; width = j;
    int k, l;    // channel = k; intensity value =255;

    // convert char to int
    for (k=0; k<=2; k++) {
        for (i=0; i<=399; i++) {
            for (j=0; j<=399; j++) {
                Imagedata_int[i][j][k] = (int) Imagedata[i][j][k];
            }
        }
    }

    // find the number of pixel of each grey-value
    for (k=0; k<=2; k++) {
        for (i=0; i<=399; i++) {
            for (j=0; j<=399; j++) {
                pixel_num[k][Imagedata_int[i][j][k]] = pixel_num[k][Imagedata_int[i][j][k]] + 1;
            }       // Q: what does this mean ...... ?
        }
    }

    // use method B: culmulative probablity to equalize
    // use bucket filling
    // step1: calculate how many pixels go to each bucket (intensity)
    int PixelPerBucket = 625;  // 400*400/256 = 625 pixels go to each intensity

    // step2: assign pixels in original image to corresponding bucket
    int index_i[Size*Size];    // all the pixel location of intensity l
    int index_j[Size*Size];    // Question: Size*Size can run; 400*400 can not run
    int ii, jj;
    int N = 0;

    for(k=0; k<=2; k++){
        // sort pixel according to intensity value
        ii = 0;
        jj = 0;
        for(l=0; l<=255; l++){
            for(i=0; i<=399; i++){
                for(j=0; j<=399; j++){
                    if(Imagedata_int[i][j][k] == l)
                    {
                        index_i[ii] = i;   // find all the location of pixel of intensity = l
                        index_j[jj] = j;
                        ii = ii + 1;          // index move forward by (1,1)
                        jj = jj + 1;
                    }
                }
            }
        }
        // put 625 pixels into each intensity

        for(l=0; l<=255; l++){
            for(i=N; i<= N+PixelPerBucket; i++){
                Imagedata[index_i[i]][index_j[i]][k] = (unsigned char)(l);
            }
            N = N + PixelPerBucket;
        }

    }

    // write the cumulative histogram
    for(l=255; l>=0; l--){                           //记得改回来，本来是l++
        cout<< pixel_num[1][l]<<";" <<endl;
    }

    /*
    // use method A: transfer function to equalize
    // find the CDF function
    int count;
    for (k=0; k<=2; k++){
        for (i=0; i<=255; i++) {
            count = 0;
            for (j=0; j<=i; j++) {
                count = count + pixel_num[k][j];
            }
            pixel_cumu[k][i] = count;    // cumulate pixel_num at each intensity from 1-255
        }
    }


    // mapping CDF_old to CDF_hist
    for (k=0; k<=2; k++){
        for(l=0; l<=255; l++){
            for (i=0; i<=399; i++) {
                for (j=0; j<=399; j++) {
                    if (Imagedata_int[i][j][k] == l)
                    {
                        Imagedata_hist[i][j][k] = (unsigned char)(255 * pixel_cumu[k][l] / (Size * Size));
                        // to normalize: (...) / (Size*Size)
                        // mapping rule: x' = 255 * y
                    }

                }
            }
        }
    }
    */

    ///////////////////////// END OF MY PROCESSING CODE /////////////////////////
    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    // display equalized image: Imagedata_hist
    fwrite(Imagedata, sizeof(unsigned char), Size*Size*3, file);
    fclose(file);

    return 0;
}
