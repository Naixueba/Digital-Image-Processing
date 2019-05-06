// HW2.Prob.2.(a)
// random thresholding
// Input & Usage: g++ -o random random.cpp
//                ./random bridge.raw bridge_random.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#define random(x) (rand()%x)

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int Size_r = 600;
    int Size_c = 400;

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
    fread(Imagedata, sizeof(unsigned char), 600*400*1, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k, l;
    unsigned char T[Size_r][Size_c][1];

    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(k=0;k<BytesPerPixel;k++){
                T[i][j][k] = random(255);
            }
        }
    }

    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(k=0;k<BytesPerPixel;k++){
                if(Imagedata[i][j][k]>T[i][j][k])
                {
                    Imagedata[i][j][k]=255;
                }
                else
                {
                    Imagedata[i][j][k]=0;
                }
            }
        }
    }


    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), 600*400*1, file);
    fclose(file);

    return 0;
}
