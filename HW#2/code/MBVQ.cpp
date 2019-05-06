// Prob.2.(c) Color Image Error Diffusion
// MBVQ
// Input & Usage: g++ -o MBVQ MBVQ.cpp
//                ./MBVQ bird.raw bird_MBVQ.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// method to get MBVQ
string getMBVQ(float R, float G, float B){
    string MBVQ = "AAAA";
    if((R+G) > 1){
        if((G+B) > 1){
            if((R+G+B) > 2) { MBVQ = "CMYW"; }
            else { MBVQ = "MYGC"; }
        }
        else { MBVQ = "RGMY"; }
    }
    else{
        if((G+B) <= 1) {
            if((R+G+B) <= 1) { MBVQ = "KRGB"; }
            else { MBVQ = "RGBM"; }
        }
        else { MBVQ = "CMGB"; }
    }
    return MBVQ;
}
// method to get vertex
char getVertex(string MBVQ, float R, float G, float B){
    char vertex = 'r';
    // No.1 for CMYW
    if(MBVQ == "CMYW")
    {
        vertex = 'w';
        if(B < 0.5){
            if(B <= R){
                if(B <= G){
                    vertex = 'y'; } } }
        if(G < 0.5){
            if(G <= B){
                if(G <= R){
                    vertex = 'm'; } } }
        if(R < 0.5){
            if(R <= B){
                if(R <= G){
                    vertex = 'c'; } } }
    }
    // No.2 for MYGC
    if(MBVQ == "MYGC")
    {
        vertex = 'm';
        if (G >= B) {
            if (R >= B) {
                if (R >= 0.5) {vertex = 'y';}
                else {vertex = 'g';}
            }
        }
        if (G >= R){
            if (B >= R){
                if (B >= 0.5){vertex = 'c';}
                else{vertex = 'g';}
            }
        }
    }
    // No.3 for RGMY
    if (MBVQ == "RGMY"){
        if (B > 0.5){
            if (R > 0.5) {
                if (B >= G) { vertex = 'm'; }
                else { vertex = 'y'; }
            }
            else{
                if (G > B + R){ vertex = 'g'; }
                else{vertex = 'm';}
                }
        }
        else{
            if (R >= 0.5){
                if (G >= 0.5){vertex = 'y';}
                else{vertex = 'r';}
            }
            else{
                if (R >= G){vertex = 'r';}
                else{vertex = 'g';}
            }
        }
    }
    // No.4 for KRGB
    if (MBVQ == "KRGB"){
        vertex = 'k';
        if (B > 0.5) {
            if (B >= R) {
                if (B >= G) { vertex = 'b'; }
            }
        }
        if (G > 0.5){
            if (G >= B){
                if (G >= R){ vertex = 'g'; }
            }
        }
        if (R > 0.5){
            if (R >= B){
                if (R >= G){vertex = 'r';}
            }
        }
    }
    // No.5 for RGBM
    if (MBVQ == "RGBM"){
        vertex = 'g';
        if (R > G){
            if (R >= B){
                if (B < 0.5){vertex = 'r';}
                else{vertex = 'm';}
            }
        }
        if (B > G){
            if (B >= R){
                if (R < 0.5){vertex = 'b';}
                else{vertex = 'm';}
            }
        }
    }
    // No.6 for CMGB
    if (MBVQ == "CMGB"){
        if (B > 0.5){
            if ( R > 0.5){
                if (G >= R){vertex = 'c';}
                else{vertex = 'm';}
            }
            else{
                if (G > 0.5){vertex = 'c';}
                else{vertex = 'b';}
            }
        }
        else{
            if ( R > 0.5){
                if (R - G + B >= 0.5){vertex = 'm';}
                else{vertex = 'g';}
            }
            else{
                if (G >= B){vertex = 'g';}
                else{vertex = 'b';}
            }
        }
    }
    return vertex;
}

// main function start
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
    //float f[Size_r][Size_c][BytesPerPixel];
    int b1,b2,b3;
    double e1,e2,e3;
    float Imagedata_float[Size_r][Size_c][BytesPerPixel];
    float Imagedata_RGB[Size_r][Size_c][BytesPerPixel];
    string MBVQ;
    char vertex[Size_r][Size_c];
    //string vertex[Size_r][Size_c][BytesPerPixel];

    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(k=0;k<BytesPerPixel;k++){
                // F(i, j)
                Imagedata_float[i][j][k] = (float)Imagedata[i][j][k];
                //f[i][j][k] = Imagedata_float[i][j][k]/255;
                Imagedata_RGB[i][j][k] = Imagedata_float[i][j][k]/255;
            }
        }
    }

    for(i=0;i<Size_r;i++){
        // scan from left
        if(i%2==0){
            for(j=0;j<Size_c;j++) {
                // find MBVQ
                MBVQ = getMBVQ(Imagedata_RGB[i][j][0], Imagedata_RGB[i][j][1], Imagedata_RGB[i][j][2]);
                //cout<< MBVQ <<endl;

                // new RGB(i,j) = RGB(i,j) + e
                Imagedata_RGB[i][j][0] = Imagedata_RGB[i][j][0] + e1;
                Imagedata_RGB[i][j][1] = Imagedata_RGB[i][j][1] + e2;
                Imagedata_RGB[i][j][2] = Imagedata_RGB[i][j][2] + e3;

                // find new vertex
                vertex[i][j] = getVertex(MBVQ, Imagedata_RGB[i][j][0], Imagedata_RGB[i][j][1], Imagedata_RGB[i][j][2]);
                cout<< vertex[i][j] <<endl;
                // assign the pixel with corresponding value
                if (vertex[i][j] == 'k') { b1 = 0; b2 = 0; b3 = 0; }
                if (vertex[i][j] == 'b') { b1 = 0; b2 = 0; b3 = 1; }
                if (vertex[i][j] == 'g') { b1 = 0; b2 = 1; b3 = 0; }
                if (vertex[i][j] == 'r') { b1 = 1; b2 = 0; b3 = 0; }
                if (vertex[i][j] == 'c') { b1 = 0; b2 = 1; b3 = 1; }
                if (vertex[i][j] == 'm') { b1 = 1; b2 = 0; b3 = 1; }
                if (vertex[i][j] == 'y') { b1 = 1; b2 = 1; b3 = 0; }
                if (vertex[i][j] == 'w') { b1 = 1; b2 = 1; b3 = 1; }

                e1 = Imagedata_RGB[i][j][0] - b1;
                e2 = Imagedata_RGB[i][j][1] - b2;
                e3 = Imagedata_RGB[i][j][2] - b3;

                // diffuse the error
                if (j < (Size_c - 1)) {
                    Imagedata_RGB[i][j+1][0] = Imagedata_RGB[i][j+1][0] + 7*e1/16;
                    Imagedata_RGB[i][j+1][1] = Imagedata_RGB[i][j+1][1] + 7*e2/16;
                    Imagedata_RGB[i][j+1][2] = Imagedata_RGB[i][j+1][2] + 7*e3/16;
                }
                // right bottom pixel
                if (j < (Size_c - 1) && i < (Size_r - 1)) {
                    Imagedata_RGB[i+1][j+1][0] = Imagedata_RGB[i+1][j+1][0] + 1*e1/16;
                    Imagedata_RGB[i+1][j+1][1] = Imagedata_RGB[i+1][j+1][1] + 1*e2/16;
                    Imagedata_RGB[i+1][j+1][2] = Imagedata_RGB[i+1][j+1][2] + e1*3/16;
                }
                // bottom pixel
                if (i < (Size_r - 1)) {
                    Imagedata_RGB[i+1][j][0] = Imagedata_RGB[i+1][j][0] + 5*e1/16;
                    Imagedata_RGB[i+1][j][1] = Imagedata_RGB[i+1][j][1] + 5*e2/16;
                    Imagedata_RGB[i+1][j][2] = Imagedata_RGB[i+1][j][2] + 5*e3/16;
                }
                // left bottom pixel
                if (i < (Size_r - 1) && j > 0) {
                    Imagedata_RGB[i+1][j-1][0] = Imagedata_RGB[i+1][j-1][0] + 3*e1/16;
                    Imagedata_RGB[i+1][j-1][1] = Imagedata_RGB[i+1][j-1][1] + 3*e2/16;
                    Imagedata_RGB[i+1][j-1][2] = Imagedata_RGB[i+1][j-1][2] + 3*e3/16;
                }
                e1=0;e2=0;e3=0;
            }
        }
        // scan from right
        else{
            for(j=Size_c-1;j>=0;j--) {
                // find MBVQ
                MBVQ = getMBVQ(Imagedata_RGB[i][j][0], Imagedata_RGB[i][j][1], Imagedata_RGB[i][j][2]);
                //cout<< MBVQ <<endl;

                // new RGB(i,j) = RGB(i,j) + e
                Imagedata_RGB[i][j][0] = Imagedata_RGB[i][j][0] + e1;
                Imagedata_RGB[i][j][1] = Imagedata_RGB[i][j][1] + e2;
                Imagedata_RGB[i][j][2] = Imagedata_RGB[i][j][2] + e3;

                // find new vertex
                vertex[i][j] = getVertex(MBVQ, Imagedata_RGB[i][j][0], Imagedata_RGB[i][j][1], Imagedata_RGB[i][j][2]);
                cout<< vertex[i][j] <<endl;
                // assign the pixel with corresponding value
                if (vertex[i][j] == 'k') { b1 = 0; b2 = 0; b3 = 0; }
                if (vertex[i][j] == 'b') { b1 = 0; b2 = 0; b3 = 1; }
                if (vertex[i][j] == 'g') { b1 = 0; b2 = 1; b3 = 0; }
                if (vertex[i][j] == 'r') { b1 = 1; b2 = 0; b3 = 0; }
                if (vertex[i][j] == 'c') { b1 = 0; b2 = 1; b3 = 1; }
                if (vertex[i][j] == 'm') { b1 = 1; b2 = 0; b3 = 1; }
                if (vertex[i][j] == 'y') { b1 = 1; b2 = 1; b3 = 0; }
                if (vertex[i][j] == 'w') { b1 = 1; b2 = 1; b3 = 1; }

                e1 = Imagedata_RGB[i][j][0] - b1;
                e2 = Imagedata_RGB[i][j][1] - b2;
                e3 = Imagedata_RGB[i][j][2] - b3;

                // diffuse the error
                // left pixel
                if(j>=1){
                    Imagedata_RGB[i][j-1][0] = Imagedata_RGB[i][j-1][0] + 7*e1/16;
                    Imagedata_RGB[i][j-1][1] = Imagedata_RGB[i][j-1][1] + 7*e2/16;
                    Imagedata_RGB[i][j-1][2] = Imagedata_RGB[i][j-1][2] + 7*e3/16;
                }
                // left bottom
                if(j>=1 && i<(Size_r-1)){
                    Imagedata_RGB[i+1][j-1][0] = Imagedata_RGB[i+1][j-1][0] + 1*e1/16;
                    Imagedata_RGB[i+1][j-1][1] = Imagedata_RGB[i+1][j-1][1] + 1*e2/16;
                    Imagedata_RGB[i+1][j-1][2] = Imagedata_RGB[i+1][j-1][2] + 1*e3/16;
                }
                // bottom pixel
                if(i<(Size_r-1)){
                    Imagedata_RGB[i+1][j][0] = Imagedata_RGB[i+1][j][0] + 5*e1/16;
                    Imagedata_RGB[i+1][j][1] = Imagedata_RGB[i+1][j][1] + 5*e2/16;
                    Imagedata_RGB[i+1][j][2] = Imagedata_RGB[i+1][j][2] + 5*e3/16;
                }
                // right bottom pixel
                if((i<Size_r-1) && (j<Size_c-1)){
                    Imagedata_RGB[i+1][j+1][0] = Imagedata_RGB[i+1][j+1][0] + 3*e1/16;
                    Imagedata_RGB[i+1][j+1][1] = Imagedata_RGB[i+1][j+1][1] + 3*e2/16;
                    Imagedata_RGB[i+1][j+1][2] = Imagedata_RGB[i+1][j+1][2] + 3*e3/16;
                }
                e1=0;e2=0;e3=0;
            }
        }
        // complete
    }

    // use Imagedata_RGB to binarize output
    for(i=0;i<Size_r;i++){
        for(j=0;j<Size_c;j++){
            for(k=0;k<BytesPerPixel;k++){
                if(Imagedata_RGB[i][j][k]>0.5)
                {
                    Imagedata[i][j][k] = 255;
                }
                else
                {
                    Imagedata[i][j][k] = 0;
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
    fwrite(Imagedata, sizeof(unsigned char), 500*375*3, file);
    fclose(file);

    return 0;
}

