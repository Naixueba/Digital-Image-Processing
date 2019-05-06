// Prob.2.(b) Error Diffusion
// method A: Floyd-Steinberg
// Input & Usage: g++ -o error_diffusion_fs error_diffusion_fs.cpp
//                ./error_diffusion_fs bridge.raw bridge_fs.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int Size_r = 400;
    int Size_c = 600;

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
    fread(Imagedata, sizeof(unsigned char), 400*600*1, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k;
    int l = 0;
    double f[Size_r][Size_c][BytesPerPixel];
    int b;
    double e;
    double Imagedata_double[Size_r][Size_c][BytesPerPixel];

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


    ///////////////////////// Problems Found /////////////////////////
    // 1. in for-loop, when exceed, i/j jump, causing jump-line operation???
    // 2. switch Size_r and Size_c, the vertical blank line disappear
    // 3. i++ -> i=i+2; better, but still has horizontal lines
    ///////////////////////// Question Found /////////////////////////

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), 400*600*1, file);
    fclose(file);

    return 0;
}

