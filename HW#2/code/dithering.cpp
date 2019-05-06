// hw2 prob.2.(a)
// constant threshold
// and dithering matrix of 2*2, 4*4, 8*8, 32*32
// Input & Usage: g++ -o dithering dithering.cpp
//                ./dithering bridge.raw bridge_dithering_2.raw bridge_dithering_4.raw bridge_dithering_8.raw bridge_dithering_32.raw bridge_dithering_constant.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
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

    // read in image
    if(!(file=fopen(argv[1], "rb"))){
        cout << "Cannot open file" << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 600*400*1, file);
    fclose(file);

    ////////////////////////// MY PROCESSING CODE ///////////////////////////
    int i, j, k, l;
    int ch;
    double T;
    int I_2 [2][2] = {{1,2}, {3,0}};
    int I_4 [4][4] ={{5,9,6,10},{13,1,14,2},{7,11,4,8},{15,3,12,0}};
    int I_8 [8][8] = {{21,37,25,41,22,38,26,42},{53,5,57,9,54,6,58,10},{29,45,17,33,30,46,18,34},{61,13,49,1,62,14,50,2},{23,39,27,43,20,36,24,40},{55,7,59,11,52,4,56,8},{31,47,19,35,28,44,16,32},{63,15,51,3,60,12,48,0}};
    int I_16 [16][16];
    int I_32 [32][32];

    unsigned char img_2[Size_r][Size_c][BytesPerPixel];
    unsigned char img_4[Size_r][Size_c][BytesPerPixel];
    unsigned char img_8[Size_r][Size_c][BytesPerPixel];
    unsigned char img_32[Size_r][Size_c][BytesPerPixel];

    // copy the input image data
    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(ch=0;ch<BytesPerPixel;ch++){
                img_2[i][j][ch]=Imagedata[i][j][ch];
                img_4[i][j][ch]=Imagedata[i][j][ch];
                img_8[i][j][ch]=Imagedata[i][j][ch];
                img_32[i][j][ch]=Imagedata[i][j][ch];
            }
        }
    }

    // I2
    for(i=0;i<Size_r;i++)
    {
        for(j=0;j<Size_c;j++)
        {
            for(ch=0;ch<BytesPerPixel;ch++){
                k = i % 2;
                l = j % 2;
                T = 255 * (I_2[k][l] + 0.5) / 4;
                if(img_2[i][j][ch] <= T)
                {
                    img_2[i][j][ch] = 0;
                }
                else
                {
                    img_2[i][j][ch] = 255;
                }
            }
        }
    }

    //I4
    for(i=0;i<Size_r;i++)
    {
        for(j=0;j<Size_c;j++)
        {
            for(ch=0;ch<BytesPerPixel;ch++){
                k = i % 4;
                l = j % 4;
                T = 255 * (I_4[k][l] + 0.5) / 16;
                if(img_4[i][j][ch] <= T)
                {
                    img_4[i][j][ch] = 0;
                }
                else
                {
                    img_4[i][j][ch] = 255;
                }
            }
        }
    }

    //I8
    for(i=0;i<Size_r;i++)
    {
        for(j=0;j<Size_c;j++)
        {
            for(ch=0;ch<BytesPerPixel;ch++){
                k = i % 8;
                l = j % 8;
                T = 255 * (I_8[k][l] + 0.5) / 64;
                if(img_8[i][j][ch] <= T)
                {
                    img_8[i][j][ch] = 0;
                }
                else
                {
                    img_8[i][j][ch] = 255;
                }
            }
        }
    }

    // calculate I_16 & I_32
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            I_16[i][j] = I_8[i][j]*4 + 1;
            I_16[i][j+8] = I_8[i][j]*4 + 2;
            I_16[i+8][j] = I_8[i][j]*4 + 3;
            I_16[i+8][j+8] = I_8[i][j]*4 + 0;
        }
    }
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            I_32[i][j] = I_16[i][j]*4 + 1;
            I_32[i][j+16] = I_16[i][j]*4 + 2;
            I_32[i+16][j] = I_16[i][j]*4 + 3;
            I_32[i+16][j+16] = I_16[i][j]*4 + 0;
        }
    }

    //I32
    for(i=0;i<Size_r;i++)
    {
        for(j=0;j<Size_c;j++)
        {
            for(ch=0;ch<BytesPerPixel;ch++){
                k = i % 32;
                l = j % 32;
                T = 255 * (I_32[k][l] + 0.5) / 1024;
                if(img_32[i][j][ch] <= T)
                {
                    img_32[i][j][ch] = 0;
                }
                else
                {
                    img_32[i][j][ch] = 255;
                }
            }
        }
    }

    // binary dithering with constant threshold
    unsigned char img_const_bi[Size_r][Size_c][ch];
    for(i=0;i<Size_r;i++)
    {
        for(j=0;j<Size_c;j++)
        {
            for(ch=0;ch<BytesPerPixel;ch++){
                if(Imagedata[i][j][ch] > 128){
                    Imagedata[i][j][ch] = 255;
                }
                else{
                   Imagedata[i][j][ch] = 0;
                }
            }
        }
    }

    //////////////////////////// END OF MY CODE ////////////////////////////
    // write I_2
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(img_2, sizeof(unsigned char), 400*600*1, file);
    fclose(file);
    // write I_4
    if (!(file=fopen(argv[3],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(img_4, sizeof(unsigned char), 400*600*1, file);
    fclose(file);
    // write I_8
    if (!(file=fopen(argv[4],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(img_8, sizeof(unsigned char), 400*600*1, file);
    fclose(file);
    // write I_32
    if (!(file=fopen(argv[5],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(img_32, sizeof(unsigned char), 400*600*1, file);
    fclose(file);
    // write constant threshold
    if (!(file=fopen(argv[6],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(img_const_bi, sizeof(unsigned char), 400*600*1, file);
    fclose(file);

    return 0;
}




