// HW#3 Prob.2.(c)
// rice counting and size comparison
// Submissioin Date: March.03
// USCID: 8372-7611-20
// Name: Wenjun Li


#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 3;
    int size_r = 500;
    int size_c = 690;

    // Allocate image data array
    unsigned char Imagedata[size_r][size_c][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen("/Users/wenjun/ee569/hw3/rice.raw","rb"))) {
        cout << "Cannot open file: " <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 500*690*3, file);
    fclose(file);
    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k;
    int count = 0;
    double temp;
    unsigned char image_gray[size_r][size_c][1];
    unsigned char image_binary[size_r][size_c][1];

    for(i=0;i<size_r;i++){
        for(j=0;j<size_c;j++){
            temp = (double)(299*Imagedata[i][j][0] + 587*Imagedata[i][j][1] + 114*Imagedata[i][j][2])/1000;
            image_gray[i][j][0] = (unsigned char)temp;
        }
    }

    // convert gray image to binary image
    for(i=0;i<size_r;i++){
        for(j=0;j<size_c;j++){
            if( i<= 395)
            {
                if (image_gray[i][j][0] >= 60 && image_gray[i][j][0] <= 80) {
                    image_binary[i][j][0] = 0;
                } else { image_binary[i][j][0] = 255; }
            }
        }
    }
    // type J is special, need to be treated differently
    for(i=396;i<size_r;i++) {
        for (j = 170; j < 360; j++) {
            if (image_gray[i][j][0] == 72 || image_gray[i][j][0] == 71 || image_gray[i][j][0] == 73 || image_gray[i][j][0] == 74 || image_gray[i][j][0] == 70 || image_gray[i][j][0] == 75 || image_gray[i][j][0] == 76 || image_gray[i][j][0] == 80) {
                image_binary[i][j][0] = 0;
            }
            else {
                image_binary[i][j][0] = 255;
            }
        }
        for (j = 360; j < size_c; j++) {
            if (image_gray[i][j][0] >= 60 && image_gray[i][j][0] <= 80) {
                image_binary[i][j][0] = 0;
            } else { image_binary[i][j][0] = 255; }
        }
    }

    // remove those isolated white dot
    for(i=1;i<size_r-1;i++){
        for(j=1;j<size_c-1;j++){
            // single white dot
            if(image_binary[i][j][0] == 255 && image_binary[i-1][j][0] == 0 && image_binary[i+1][j][0] == 0  && image_binary[i][j-1][0] == 0 && image_binary[i][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
            // 2-connected white dot
            if(image_binary[i][j][0] == 255 && image_binary[i][j+1][0] == 255 && image_binary[i][j-1][0] == 0  && image_binary[i][j+2][0] == 0 && image_binary[i+1][j][0] == 0 && image_binary[i-1][j][0] == 0 && image_binary[i-1][j+1][0] == 0 && image_binary[i+1][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
            if(image_binary[i][j][0] == 255 && image_binary[i+1][j][0] == 255 && image_binary[i-1][j][0] == 0  && image_binary[i+2][j][0] == 0 && image_binary[i][j-1][0] == 0 && image_binary[i][j+1][0] == 0 && image_binary[i+1][j-1][0] == 0 && image_binary[i+1][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
            // 3-connented
            if(image_binary[i][j][0] == 255 && image_binary[i+1][j][0] == 255 && image_binary[i+2][j][0] == 255  && image_binary[i][j-1][0] == 0 && image_binary[i][j+1][0] == 0 && image_binary[i-1][j][0] == 0 && image_binary[i+2][j-1][0] == 0 && image_binary[i+1][j-1][0] == 0 && image_binary[i+2][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
            if(image_binary[i][j][0] == 255 && image_binary[i][j+1][0] == 255 && image_binary[i+1][j+1][0] == 255  && image_binary[i][j-1][0] == 0 && image_binary[i+1][j][0] == 0 && image_binary[i][j+2][0] == 0 && image_binary[i-1][j+1][0] == 0 && image_binary[i+1][j+2][0] == 0 && image_binary[i+2][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
            if(image_binary[i][j][0] == 255 && image_binary[i][j+1][0] == 255 && image_binary[i][j+2][0] == 255  && image_binary[i][j-1][0] == 0 && image_binary[i][j+3][0] == 0 && image_binary[i-1][j][0] == 0 && image_binary[i+1][j][0] == 0 && image_binary[i-1][j+1][0] == 0 && image_binary[i+1][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
            // 4-connented
            if(image_binary[i][j][0] == 255 && image_binary[i][j+1][0] == 255 && image_binary[i][j+2][0] == 255  && image_binary[i+1][j][0] == 255 && image_binary[i-1][j][0] == 0 && image_binary[i][j-1][0] == 0 && image_binary[i+1][j+1][0] == 0)
            {
                image_binary[i][j][0] = 0;
            }
        }
    }


    // the background pixel value is 72
    int area_A; int area_B; int area_C; int area_D; int area_E; int area_F;
    int area_G; int area_H; int area_I; int area_J; int area_K;

    // count area of type A, B, C
    for(i=0;i<155;i++){
        for(j=0;j<230;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_A = count/5; count = 0;
    for(i=0;i<155;i++){
        for(j=230;j<490;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_B = count/5; count = 0;
    for(i=0;i<155;i++){
        for(j=490;j<690;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_C = count/5; count = 0;

    // count area of type D, E, F
    for(i=155;i<275;i++){
        for(j=0;j<230;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_D = count/5; count = 0;
    for(i=155;i<275;i++){
        for(j=230;j<490;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_E = count/5; count = 0;
    for(i=155;i<275;i++){
        for(j=490;j<690;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_F = count/5; count = 0;

    // count area of type G, H, I
    for(i=275;i<395;i++){
        for(j=0;j<230;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_G = count/5; count = 0;
    for(i=275;i<395;i++){
        for(j=230;j<490;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_H = count/5; count = 0;
    for(i=275;i<395;i++){
        for(j=490;j<690;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_I = count/5; count = 0;

    // count area of type J, K
    for(i=395;i<500;i++){
        for(j=0;j<360;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_J = count/5; count = 0;
    for(i=395;i<500;i++){
        for(j=360;j<690;j++){
            if(image_binary[i][j][0] != 0){
                count++; }}}
    area_K = count/5; count = 0;

    cout<<"Size_A"<<"\t"<<"Size_B"<<"\t"<<"Size_C"<<"\t"<<"Size_D"<<"\t"<<"Size_E"<<"\t"<<"Size_F"<<"\t"<<"Size_G"<<"\t"<<"Size_H"<<"\t"<<"Size_I"<<"\t"<<"Size_J"<<"\t"<<"Size_K"<<endl;
    cout<<area_A<<"\t"<<"\t"<<area_B<<"\t"<<"\t"<<area_C<<"\t"<<"\t"<<area_D<<"\t"<<"\t"<<area_E<<"\t"<<"\t"<<area_F<<"\t"<<"\t"<<area_G<<"\t"<<"\t"<<area_H<<"\t"<<"\t"<<area_I<<"\t"<<"\t"<<area_J<<"\t"<<"\t"<<area_K<<endl;


    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    if (!(file=fopen("/Users/wenjun/ee569/hw3/rice_binary.raw","wb"))) {
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(image_binary, sizeof(unsigned char), 500*690*1, file);
    fclose(file);
    return 0;
}
