//(1) Wenjun Li//(2) 8372761120//(3) wenjunli@usc.edu//(4) Jan.22,2019

// HW1.2.(a).(2) Image Denoising -- psnr
// psnr: calculate & plot the peak-to-signal-noise

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

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
    // noisy image & filtered image
    unsigned char noisy_image[Size][Size][BytesPerPixel];
    unsigned char filtered_image[Size][Size][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(noisy_image, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    if (!(file=fopen(argv[2],"rb"))) {
        cout << "Cannot open file: " << argv[2] <<endl;
        exit(1);
    }
    fread(filtered_image, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k, l;      // i=length, j=width, k=channel, l=intensity
    double sum, mse, psnr;
    sum = 0;
    for(k=0;k<BytesPerPixel;k++){
        for(i=0;i<Size;i++){
            for(j=0;j<Size;j++){
                // calculate difference between noisy image & filtered image
                sum = sum + (noisy_image[i][j][k]-filtered_image[i][j][k])*(noisy_image[i][j][k]-filtered_image[i][j][k]);
            }
        }
    }
    // use formula to compute mse & psnr
    mse = sum/(Size*Size*BytesPerPixel);
    psnr = 10*log10(255*255/mse);

    // write psnr array
    cout<< psnr <<endl;

    ///////////////////////// Some Questions ///////////////////////
    // 1. can i use char to compute? and get double from char?

    ///////////////////////// To be solved /////////////////////////


    ///////////////////////// END OF MY PROCESSING CODE /////////////////////////
}
