// Prob.2.(c) Color Image Error Diffusion
// Separable error diffusion using Floyd-Steinberg dithering matrix
// Input & Usage: g++ -o separable_ED separable_ED.cpp
//                ./separable_ED bird.raw bird_separable_ED.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 3;
    int Size_r = 375;
    int Size_c = 500;

    // Check for proper syntax
    if (argc < 3){
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
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 500*375*3, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k;
    int l = 0;
    float f[Size_r][Size_c][BytesPerPixel];
    int b;
    double e;
    float Imagedata_double[Size_r][Size_c][BytesPerPixel];

    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(k=0;k<BytesPerPixel;k++){
                // F(i, j)
                Imagedata_double[i][j][k] = (double)Imagedata[i][j][k];
                f[i][j][k] = Imagedata_double[i][j][k]/255;
            }
        }
    }

    // Floyd-Steinberg Error Diffusion
    for(i=0;i<Size_r;i=i+1)
    {
        // scan from left
        if(l%2==0)
        {
            for (j = 0; j < Size_c; j++)
            {
                for (k = 0; k < BytesPerPixel; k++)
                {
                    // calculate b & e
                    // b[i ,j]
                    if (f[i][j][k] > 0.5) { b = 1; }
                    else { b = 0; }
                    // e= f - b
                    e = f[i][j][k] - b;
                    // diffuse the error
                    // right pixel
                    if (j < (Size_c - 1)) {
                        //Imagedata_double[i][j + 1][k] = Imagedata_double[i][j + 1][k] + 7 * e / 16;
                        f[i][j+1][k] = f[i][j+1][k] + 7*e/16;
                    }
                    // right bottom pixel
                    if (j < (Size_c - 1) && i < (Size_r - 1)) {
                        //Imagedata_double[i + 1][j + 1][k] = Imagedata_double[i + 1][j + 1][k] + e / 16;
                        f[i+1][j+1][k] = f[i+1][j+1][k] + e/16;
                    }
                    // bottom pixel
                    if (i < (Size_r - 1)) {
                        //Imagedata_double[i + 1][j][k] = Imagedata_double[i + 1][j][k] + 5 * e / 16;
                        f[i+1][j][k] = f[i+1][j][k] + 5*e/16;
                    }
                    // left bottom pixel
                    if (i < (Size_r - 1) && j > 0) {
                        //Imagedata_double[i + 1][j - 1][k] = Imagedata_double[i + 1][j - 1][k] + 3 * e / 16;
                        f[i+1][j-1][k] = f[i+1][j-1][k] + 3*e/16;
                    }
                }
            }
            l++;
        }

            // scan from right
        else{
            for(j=(Size_c-1);j>=0;j--){
                for(k=0;k<BytesPerPixel;k++){
                    // calculate b & e
                    // b[i ,j]
                    if(f[i][j][k] > 0.5) {b = 1;}
                    else {b = 0;}
                    // e= f - b
                    e = f[i][j][k] - b;

                    // diffuse the error
                    // left pixel
                    if(j>=1){
                        //Imagedata_double[i][j-1][k] = Imagedata_double[i][j-1][k]+7*e/16;
                        f[i][j-1][k] = f[i][j-1][k] + 7*e/16;
                    }
                    // left bottom
                    if(j>=1 && i<(Size_r-1)){
                        //Imagedata_double[i+1][j-1][k] = Imagedata_double[i+1][j-1][k]+1*e/16;
                        f[i+1][j-1][k] = f[i+1][j-1][k] + 1*e/16;
                    }
                    // bottom pixel
                    if(i<(Size_r-1)){
                        //Imagedata_double[i+1][j][k] = Imagedata_double[i+1][j][k]+5*e/16;
                        f[i+1][j][k] = f[i+1][j][k] + 5*e/16;
                    }
                    // right bottom pixel
                    if((i<Size_r-1) && (j<Size_c-1)){
                        //Imagedata_double[i+1][j+1][k] = Imagedata_double[i+1][j+1][k]+3*e/16;
                        f[i+1][j+1][k] = f[i+1][j+1][k] + 3*e/16;
                    }
                }
            }
            l++;
        }

    }// error diffusion complete


    // convert Imagedata_double to unsigned char
    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(k=0;k<BytesPerPixel;k++){
                if(f[i][j][k]>0.5)
                {
                    Imagedata[i][j][k] = 255;
                }
                else
                {
                    Imagedata[i][j][k] = 0;
                }
                //Imagedata[i][j][k] = (unsigned char)Imagedata_double[i][j][k];
            }
        }
    }

    ///////////////////////// Problems & Debug /////////////////////////
    // 1. color image cannot use double to define; use float

    ///////////////////////// Record /////////////////////////

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), 500*375*3, file);
    fclose(file);

    return 0;
}

