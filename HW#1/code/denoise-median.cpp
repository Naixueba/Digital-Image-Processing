//(1) Wenjun Li//(2) 8372761120//(3) wenjunli@usc.edu//(4) Jan.22,2019

// HW1.2.(b) -- filter mixed noise of uniform & inpulse noise
// median filter: filter impulse noise

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
    int Size = 256;

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
    unsigned char Imagedata[Size][Size][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 256*256*3, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int i, j, k, l;    // i=image row index, j=image column index, k=neighbor row index, i=neighbor row index
    int ch;       // ch = color channel; ints = intensity value of pixel
    int N = 3;         // window size N= 3, 5, 7, ...
    unsigned char imgLength[Size+N-1][Size][BytesPerPixel];
    unsigned char imgExtend[Size+N-1][Size+N-1][BytesPerPixel];
    int window[N][N]; // window to store N*N element
    int sort_window[N*N][BytesPerPixel];  // sorted window elements, from min -> max

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


        // do median filtering
        // start point (((N-1)/2), ((N-1)/2)), end point((Size+(N-1)/2), (Size+(N-1)/2))
        for(i=(N-1)/2;i<Size+(N-1)/2;i++){
            for(j=(N-1)/2;j<Size+(N-1)/2;j++)
            {
                // get N*N window elements
                for(k=0;k<N;k++){
                    for(l=0;l<N;l++){
                        window[k][l] = imgExtend[k+i-(N-1)/2][l+j-(N-1)/2][ch];
                        sort_window[k*N+l][ch] = window[k][l];  // input sort_window element in order
                    }
                } // N*N window elements loaded


                // sort window element
                int temp = 0;
                for(k=0;k<N*N-1;k++)
                {
                    for(l=0;l<N*N-1;l++)
                    {
                       if(sort_window[l][ch] > sort_window[l+1][ch])
                        {
                            temp = sort_window[l][ch];
                            sort_window[l][ch] = sort_window[l+1][ch];
                            sort_window[l+1][ch] = temp;
                        }
                    }
                }// element in sort_window sorted, from min -> max

                // put median value [(N*N+1)/2][ch] into [i-(...)][j-(...)][ch]
                Imagedata[i-(N-1)/2][j-(N-1)/2][ch] = (unsigned char)sort_window[(N*N+1)/2][ch];
            }
        }
    }

    ///////////////////////// Questions and Debut Records ////////////////////////
    // 1. ----- (x)
    // 2.
    ///////////////////////// Solved and To Be Solved ////////////////////////////

    ///////////////////////// END OF MY PROCESSING CODE //////////////////////////
    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), 256*256*3, file);
    fclose(file);

    return 0;
}
