// HW2 Prob.1.(a)
// sobel edge detector
// Input & Usage: g++ -o sobeo sobeo.cpp
//                ./sobel Tiger.raw Tiger_sobel_Gx.raw Tiger_sobel_Gy.raw Tiger_sobel_G.raw Tiger_sobel_Threshold.raw
//                ./sobel Pig.raw Pig_sobel.raw Pig_sobel_Gy.raw Pig_sobel_G.raw Pig_sobel_Threshold.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 3;
    int size_r = 321;
    int size_c = 481;

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
    unsigned char Imagedata[size_r][size_c][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 481*321*3, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k;
    float img_grey[size_r][size_c];
    int img_grey_int[size_r][size_c];
    int gx[size_r][size_c];
    int gy[size_r][size_c];
    int g[size_r][size_c];
    int max_gx = 0; double min_gx = 0; double max_gy = 0; double min_gy = 0; double max_g = 0; double min_g = 0;
    unsigned char Gx[size_r][size_c];
    unsigned char Gy[size_r][size_c];
    unsigned char G[size_r][size_c];
    unsigned char G_Threshold_binary[size_r][size_c];
    unsigned char img_grey_out[size_r][size_c];

    // convert color image to grey image
    for(i=0;i<size_r;i++){
        for(j=0;j<size_c;j++){
            //for(k=0;k<BytesPerPixel;k++) {
              //  img_float[i][j][k] = (float) Imagedata[i][j][k];
            //}
            img_grey[i][j] = (299*Imagedata[i][j][0] + 587*Imagedata[i][j][1] + 114*Imagedata[i][j][2])/1000;
            img_grey_int[i][j] = (int)img_grey[i][j];
        }
    }
    // use sobel mask
    for(i=1;i<size_r-1;i++){
        for(j=1;j<size_c-1;j++){
            gx[i][j] = img_grey_int[i-1][j+1]+2*img_grey_int[i][j+1]+img_grey_int[i+1][j+1] - (img_grey_int[i-1][j-1]+2*img_grey_int[i][j-1]+img_grey_int[i+1][j-1]);
            gy[i][j] = img_grey_int[i-1][j-1]+2*img_grey_int[i-1][j]+img_grey_int[i-1][j+1] - (img_grey_int[i+1][j-1]+2*img_grey_int[i+1][j]+img_grey_int[i+1][j+1]);
            g[i][j] = (int) sqrt(gx[i][j]*gx[i][j] + gy[i][j]*gy[i][j]);
            // take absolute value
            gx[i][j] = abs(gx[i][j]);
            gy[i][j] = abs(gy[i][j]);
        }
    }
    // assign boundary pixel value
    for(j=0;j<size_c;j++){ gx[0][j] = 0; }
    for(j=0;j<size_c;j++){ gx[size_r-1][j] = 0; }
    for(i=0;i<size_r;i++){ g[i][0] = 0; }
    for(i=0;i<size_r;i++){ g[i][size_c-1] = 0; }
/*
    for(i=0; i<size_r; i++){
        for(j=0; j<size_c; j++) {
            // for Gx
            cout<< g[i][j] <<endl;
        }
    }
*/
    // find the max gradient and min gradient
    for(i=0; i<size_r; i++){
        for(j=0; j<size_c; j++) {
            // for Gx
            if (gx[i][j] > max_gx) { max_gx = gx[i][j]; }
            if (gx[i][j] < min_gx) { min_gx = gx[i][j]; }
            // for Gy
            if (gy[i][j] > max_gy) { max_gy = gy[i][j]; }
            if (gy[i][j] < min_gy) { min_gy = gy[i][j]; }
            // for G
            if (g[i][j] > max_g) { max_g = g[i][j]; }
            if (g[i][j] < min_g) { min_g = g[i][j]; }
        }
    }
    // assign gx, gy, g (float) to (unsigned char) Gx, Gy, G
    for(i=0; i<size_r; i++){
        for(j=0; j<size_c; j++) {
            Gx[i][j] = (unsigned char) (255*gx[i][j]/(max_gx-min_gx));
            Gy[i][j] = (unsigned char) (255*gy[i][j]/(max_gy-min_gy));
            G[i][j] = (unsigned char) (255*g[i][j]/(max_g-min_g));
            img_grey_out[i][j] = (unsigned char) img_grey_int[i][j];
        }
    }
    // apply threshold
    int T_h = 60;
    //int T_l = 70;
    for(i=0; i<size_r; i++){
        for(j=0; j<size_c; j++) {
            if(G[i][j] >= T_h) { G_Threshold_binary[i][j] = 255;}
            else {G_Threshold_binary[i][j] = 0;}
            //if(G[i][j] < T_l) { G_Threshold_binary[i][j] = 0;}
        }
    }

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    // write Gx
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Gx, sizeof(unsigned char), 481*321*1, file);
    fclose(file);
    // write Gy
    if (!(file=fopen(argv[3],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Gy, sizeof(unsigned char), 481*321*1, file);
    fclose(file);
    // write G
    if (!(file=fopen(argv[4],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(G, sizeof(unsigned char), 481*321*1, file);
    fclose(file);

    // write G_Threshold_binary
    if (!(file=fopen(argv[5],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(G_Threshold_binary, sizeof(unsigned char), 481*321*1, file);
    fclose(file);
    // write grey image
    if (!(file=fopen(argv[6],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(img_grey_out, sizeof(unsigned char), 481*321*1, file);
    fclose(file);

    return 0;
}

