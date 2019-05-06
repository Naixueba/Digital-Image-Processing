//(1) Wenjun Li//(2) 8372761120//(3) wenjunli@usc.edu//(4) Jan.22,2019

// HW1.2.(a).(3) Image Denoising -- Bilateral Filtering
// bilateral filtering

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
    unsigned char Imagedata[Size][Size][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k, l;    // i=image row index, j=image column index, k=neighbor row index, i=neighbor row index
    int ch;       // ch = color channel; ints = intensity value of pixel
    double sum, weight;// weight: store gaussian factor for each pixel; sum: store sum of gaussian factors
    int val_dif;       // intensity value difference between 2 pixels within the window
    int N = 7;         // window size N= 3, 5, 7, ...
    double sigma_c = 1;// parameter sigma-c = 1, 2, 3, 5, ...
    double sigma_s = 100;// parameter sigma-s = 1, 2, 3, 30, 50, 100, 500
    unsigned char imgLength[Size+N-1][Size][BytesPerPixel];
    unsigned char imgExtend[Size+N-1][Size+N-1][BytesPerPixel];
    int window[N][N];

    for(ch=0;ch<BytesPerPixel;ch++){
        // first, extend length by N-1 and store it in imageLength
        // copy first row and last row
        for(j=0;j<Size;j++){
            // select the row to be copied
            for(i=1;i<=(N-1)/2;i++){
                imgLength[(N-1)/2-i][j][ch] = Imagedata[i][j][ch];
                imgLength[Size-1-i+(N-1)/2][j][ch] = Imagedata[Size-1-i][j][ch];
            }
        }
        // copy the rest rows
        for(j=0;j<Size;j++){
            for(i=0;i<Size;i++){
                imgLength[i+(N-1)/2][j][ch] = Imagedata[i][j][ch];  // index[i] become index[i+(N-1)/2]
            }
        }

        // second, extend width by N-1 and store it in imageExtend
        // copy the first column and last column
        for(i=0; i<Size+N-1; i++){    // i rows become i+N-1 rows
            // select the column to be copied
            for(j=1; j<=(N-1)/2; j++){
                imgExtend[i][(N-1)/2-j][ch] = imgLength[i][j][ch];  // first l-column
                imgExtend[i][Size-1-j+(N-1)/2][ch] = imgLength[i][Size-1-j][ch]; // last l-column
            }
        }
        // fill the rest columns
        for(i=0;i<Size+N-1;i++){
            for(j=0;j<Size;j++){
                imgExtend[i][j+(N-1)/2][ch] = imgLength[i][j][ch];  //index[j] become index[j+(N-1)/2]
            }
        }// boundary extension completed, imageExtend[][][] is the new image


        // do bilateral filtering
        // start point (((N-1)/2), ((N-1)/2)), end point((Size+(N-1)/2), (Size+(N-1)/2))
        for(i=(N-1)/2;i<Size+(N-1)/2;i++){
            for(j=(N-1)/2;j<Size+(N-1)/2;j++)
            {
                //k = 0;    // set neighbor pixel index
                //l = 0;    // for every (i, j), neighbot pixel should be reset
                // get N*N window elements
                for (k=0; k<N; k++)
                {
                    for (l=0; l<N; l++)
                    {
                        window[k][l]=(int)imgExtend[i-(N-1)/2+k][j-(N-1)/2+l][ch];
                    }
                }// N*N window of neighbor pixels complete

                // set weight=0 & sum=0 for each center pixel (i, j)
                weight = 0;
                sum = 0;

                // multipuly all the window pixels by corresponding gaussian factor
                for (k=0; k<=(N-1)/2; k++) {
                    for (l=0; l<=(N-1)/2; l++) {
                        // center as ((N-1)/2, (N-1)/2), move around to neighbor pixel
                        // get Intensity difference between center and neighbor
                        // neighbor @ (+k, +l)
                        val_dif = window[(N-1)/2][(N-1)/2] - window[((N-1)/2)+k][(N-1)/2+l];
                        weight=weight+window[k][l] * (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));
                        sum = sum + (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));

                        // neighbor @ (+k, -l)
                        val_dif = window[(N-1)/2][(N-1)/2] - window[((N-1)/2)+k][(N-1)/2-l];
                        weight=weight+window[k][l] * (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));
                        sum = sum + (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));

                        // neighbor @ (-k, +l)
                        val_dif = window[(N-1)/2][(N-1)/2] - window[((N-1)/2)-k][(N-1)/2+l];
                        weight=weight+window[k][l] * (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));
                        sum = sum + (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));

                        // neighbor @ (-k, -l)
                        val_dif = window[(N-1)/2][(N-1)/2] - window[((N-1)/2)-k][(N-1)/2-l];
                        weight=weight+window[k][l] * (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));
                        sum = sum + (exp(-1*(k*k+l*l)/(2*sigma_c*sigma_c))) * (exp(-1*(val_dif*val_dif)/(2*sigma_s*sigma_s)));
                    }
                    // replace the original value with filtered value
                    Imagedata[i-(N-1)/2][j-(N-1)/2][ch]=(unsigned char)(weight/sum);
                }
                // bilateral filtering complete
            }
        }
    }

    ///////////////////////// Debug Records & Problems /////////////////////////
    // 1. in bilateral filtering [i][j] loop, end point should be [Size+(N-1)/2][Size+(N-1)/2] ----------- (x)
    // 2. fill the rest columns part, j should be "j<Size+N-1" ------ (x) ----- original version is correct...
    // 3. in select row&column copy part, new index should be "Size-1-i+(N-1)/2" ------------------------- (√)
    ///////////////////////// Solved and To Be Solved /////////////////////////

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
