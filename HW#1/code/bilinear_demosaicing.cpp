//(1) Wenjun Li//(2) 8372761120//(3) wenjunli@usc.edu//(4) Jan.22,2019

// HW1.1.(a).(1) Bilinear Demosaicing
// implement bilinear demosaicing to cat.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    //int Size = 256;
    int Size_r = 300;   // row # of image
    int Size_c = 390;   // column # of image


    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }
/*
    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 1; // default is grey image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5){
            Size = atoi(argv[4]);
        }
    }
*/
    // Allocate image data array
    unsigned char Imagedata[Size_r][Size_c][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 390*300*1, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j;                    // i = row, j = column, k = row, l = column
    int ch = BytesPerPixel - 1;                      // image channel: R, G, B
    int N = 3;                   // set N = 3, so, extend by 2 pixel
    unsigned char imgDemo[Size_r][Size_c][3];
    unsigned char imgLength[Size_r+2][Size_c][1];
    unsigned char imgExtend[Size_r+2][Size_c+2][1];

    // boundary extension by mirror reflection
        // first, extend length by N-1 and store it in imageLength
        // copy first row and last row
        for (j = 0; j < Size_c; j++) {
            // select the row to be copied
            for (i = 1; i <= (N - 1) / 2; i++) {
                imgLength[(N - 1) / 2 - i][j][ch] = Imagedata[i][j][ch];
                imgLength[Size_r - 1 - i + (N - 1) / 2][j][ch] = Imagedata[Size_r - 1 - i][j][ch];
            }
        }
        // copy the rest rows
        for (j = 0; j < Size_c; j++) {
            for (i = 0; i < Size_r; i++) {
                imgLength[i + (N - 1) / 2][j][ch] = Imagedata[i][j][ch];  // index[i] become index[i+(N-1)/2]
            }
        }

        // second, extend width by N-1 and store it in imageExtend
        // copy the first column and last column
        for (i = 0; i < Size_r + N - 1; i++) {    // i rows become i+N-1 rows
            // select the column to be copied
            for (j = 1; j <= (N - 1) / 2; j++) {
                imgExtend[i][(N - 1) / 2 - j][ch] = imgLength[i][j][ch];  // first l-column
                imgExtend[i][Size_c - 1 - j + (N - 1) / 2][ch] = imgLength[i][Size_c - 1 - j][ch]; // last l-column
            }
        }
        // fill the rest columns
        for (i = 0; i < Size_r + N - 1; i++) {
            for (j = 0; j < Size_c; j++) {
                imgExtend[i][j + (N - 1) / 2][ch] = imgLength[i][j][ch];  //index[j] become index[j+(N-1)/2]
            }
        }// boundary extension completed, imageExtend[][][] is the new image


    // do bilinear interpolation

    // red plane: four start point: (0,0), (0,1), (1,0), (1,1)
    // suppose (0,0) is red pixel, then, imply the scenario of (0,1),(1,0),(1,1)
    for (i = 0; i < Size_r; i = i + 2) {
        // start point = (0, 0)
        for (j = 0; j < Size_c; j = j + 2) {
            // red pixel itself
            imgDemo[i][j][0]=imgExtend[i+1][j+1][0];
        }
        // start point = (0, 1)
        for (j = 1; j < Size_c; j = j + 2) {
            // average of left neighbor & right neighbor
            imgDemo[i][j][0]=(0.5)*((imgExtend[i][j+1][0])+(imgExtend[i+2][j+1][0]));
        }
    }
    for (i = 1; i < Size_r; i = i + 2){
        // start point = (1,0)
        for (j = 0; j < Size_c; j = j + 2) {
            // average of up & down neighbor pixel
            imgDemo[i][j][0]=(0.5)*((imgExtend[i+1][j][0])+(imgExtend[i+1][j+2][0]));
        }
        // start point = (1,1)
        for (j = 1; j < Size_c; j = j + 2) {
            // average of four brighbor red pixel
            imgDemo[i][j][0]=(0.25)*((imgExtend[i][j][0])+(imgExtend[i][j+2][0])+(imgExtend[i+2][j][0])+(imgExtend[i+2][j+2][0]));
        }
    }

    // blue plane: four start point: (0,0), (0,1), (1,0), (1,1)
    // suppose (1,1) is blue pixel, then, imply the scenario of (0,1),(1,0),(1,1)
    for (i = 0; i < Size_r; i = i + 2) {
        // start point = (0, 0)
        for (j = 0; j < Size_c; j = j + 2) {
            // average of four neighbor blue pixel
            imgDemo[i][j][2] = (0.25)*((imgExtend[i][j][0])+(imgExtend[i][j+2][0])+(imgExtend[i+2][j][0])+(imgExtend[i+2][j+2][0]));

        }
        // start point = (0, 1)
        for (j = 1; j < Size_c; j = j + 2) {
            // average of up & down neighbor pixel
            imgDemo[i][j][2] = (0.5)*((imgExtend[i][j+1][0])+(imgExtend[i+2][j+1][0]));

        }
    }
    for (i = 1; i < Size_r; i = i + 2){
        // start point = (1,0)
        for (j = 0; j < Size_c; j = j + 2) {
            // average of left neighbor & right neighbor
            imgDemo[i][j][2] = (0.5)*((imgExtend[i+1][j][0])+(imgExtend[i+1][j+2][0]));
        }
        // start point = (1,1)
        for (j = 1; j < Size_c; j = j + 2) {
            // blue pixel itself
            imgDemo[i][j][2] = imgExtend[i+1][j+1][0];
        }
    }

    // green plane: four start point: (0,0), (0,1), (1,0), (1,1)
    // suppose (0,1) & (1,0) is green pixel, then, imply the scenario of (0,0) & (1,1)
    for (i = 0; i < Size_r; i = i + 1) {
        // start point = (0, 0)
        for (j = 0; j < Size_c; j = j + 1) {
            // average of four neighbor green pixel
            imgDemo[i][j][1] = (0.25)*((imgExtend[i][j+1][0])+(imgExtend[i+1][j][0])+(imgExtend[i+1][j+2][0])+(imgExtend[i+2][j+1][0]));

        }
        // start point = (0, 1)
        for (j = 1; j < Size_c; j = j + 1) {
            // green pixel itself
            imgDemo[i][j][1] = imgExtend[i+1][j+1][0];

        }
    }
    for (i = 1; i < Size_r; i = i + 1){
        // start point = (1,0)
        for (j = 0; j < Size_c; j = j + 1) {
            // green pixel itself
            imgDemo[i][j][1] = imgExtend[i+1][j+1][0];
        }
        // start point = (1,1)
        for (j = 1; j < Size_c; j = j + 1) {
            // average of four neighbor green pixel
            imgDemo[i][j][1] = (0.25)*((imgExtend[i][j+1][0])+(imgExtend[i+1][j][0])+(imgExtend[i+1][j+2][0])+(imgExtend[i+2][j+1][0]));
        }
        //}
        // bilinear interpolation complete


        ///////////////////////// Question and Debug Record /////////////////////////
        // 1. debug-1: still grey image...
        // 2. debug-2: image with R/G/B pixels, seperately, not colorful
        // 3. iterationi interval is not correct... but, i do not know why it's not correct....
        ///////////////////////// Solved and To Be Solved ///////////////////////////

        ///////////////////////// END OF MY PROCESSING CODE /////////////////////////
        // Write image data (filename specified by second argument) from image data matrix

        if (!(file = fopen(argv[2], "wb"))) {
            cout << "Cannot open file: " << argv[2] << endl;
            exit(1);
        }
        fwrite(imgDemo, sizeof(unsigned char), 390*300*3, file);
        fclose(file);
        return 0;
    }
}