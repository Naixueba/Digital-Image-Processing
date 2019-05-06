//(1) Wenjun Li//(2) 8372761120//(3) wenjunli@usc.edu//(4) Jan.22,2019

// HW1.2.(a) Image Denoising
// remove the noise using uniform weight function
// window size = 5*5

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int Size = 256;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

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

    // Allocate image data array
    unsigned char Imagedata[Size][Size][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    // load image: paper_uni.raw, 256*256
    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    // first, do boundary extension
    int N = 5;    // set N = 3, 5, 7, ...
    unsigned char imageExtend[Size+N-1][Size+N-1][BytesPerPixel];  // store extended matrix
    unsigned char imageLength[Size+N-1][Size][BytesPerPixel];      // store length-entended matrix
    int i, j, k, l;    // i=length, j=width, k=channel, l= loop-para
    //int window[N][N];         // set filter window size = N*N

    for(k=0;k<BytesPerPixel;k++){
        // first, extend length by N-1 and store it in imageLength
        // copy first row and last row
        for(j=0; j<Size; j++){
            // select the row to be copied
            for(l=1; l<(N+1)/2; l++){
                imageLength[(N-1)/2 - l][j][k] = Imagedata[l][j][k];  // first l-row
                imageLength[Size-1-l + (N-1)/2][j][k] = Imagedata[Size-1-l][j][k]; //last l-row
            }
        }
        // fill the rest rows
        for(j=0;j<Size;j++){
            for(i=0;i<Size;i++){
                imageLength[i+(N-1)/2][j][k] = Imagedata[i][j][k];  // index[i] become index[i+(N-1)/2]
            }
        }

        // second, extend width by N-1 and store it in imageExtend
        // copy the first column and last column
        for(i=0; i<Size+N-1; i++){    // i rows become i+N-1 rows
            // select the column to be copied
            for(l=1; l<(N+1)/2; l++){
                imageExtend[i][(N-1)/2-l][k] = imageLength[i][l][k];  // first l-column
                imageExtend[i][Size-1-l + (N-1)/2][k] = imageLength[i][Size-1-l][k]; // last l-column
            }
        }
        // fill the rest columns
        for(i=0;i<Size+N-1;i++){
            for(j=0;j<Size;j++){
                imageExtend[i][j+(N-1)/2][k] = imageLength[i][j][k];  //index[j] become index[j+(N-1)/2]
            }
        }
        // boundary extension completed, imageExtend[][][] is the new image
    }


    // apply uniform weight filter to imageExtend[][][]
    for(k=0;k<BytesPerPixel;k++){
        for(i=0;i<Size;i++){
            for(j=0;j<Size;j++){
                // do window filtering
                Imagedata[i][j][k] = 0.04*(imageExtend[i-2][j-2][k]+imageExtend[i-2][j-1][k]+imageExtend[i-2][j][k]+imageExtend[i-2][j+1][k]+imageExtend[i-2][j+2][k] +imageExtend[i-1][j-2][k]+imageExtend[i-1][j-1][k]+imageExtend[i-1][j][k]+imageExtend[i-1][j+1][k]+imageExtend[i-1][j+2][k] +imageExtend[i][j-2][k]+imageExtend[i][j-1][k]+imageExtend[i][j][k]+imageExtend[i][j+1][k]+imageExtend[i][j+2][k] +imageExtend[i+1][j-2][k]+imageExtend[i+1][j-1][k]+imageExtend[i+1][j][k]+imageExtend[i+1][j+1][k]+imageExtend[i+1][j+2][k] +imageExtend[i+2][j-2][k]+imageExtend[i+2][j-1][k]+imageExtend[i+2][j][k]+imageExtend[i+2][j+1][k]+imageExtend[i+2][j+2][k]);
            }
        }
        // uniform filtering complete
    }

    ///////////////////////// Debug & Usage Record /////////////////////////
    // 1. filter rose_color_noise, change BytesPerPixel = 3;
    // 2.
    ///////////////////////// Back to Old Version /////////////////////////

    ///////////////////////// END OF MY PROCESSING CODE /////////////////////////

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    return 0;
}


