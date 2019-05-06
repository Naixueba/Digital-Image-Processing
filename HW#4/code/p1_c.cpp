#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    FILE *file;
    int size = 510;
    int window_size = 31;
    int N = window_size;
    int n = N/2;
    int BytesPerPixel = 1;

    unsigned char Imagedata[size][size];
    unsigned char output[size][size];

    if (!(file=fopen("/Users/wenjun/ee569/hw4/comb_output.raw","rb"))) {
        cout << "Cannot open file: " <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 510*510*1, file);
    fclose(file);

    /////////////////////////////// Processing ////////////////////////////////////////
    int i, j, l;
    int pixel_value;
    int max_intensity, intensity;

    for(i=0;i<size-N;i++)
    {
        for(j=0;j<size-N;j++)
        {
            intensity = 0;
            max_intensity = 0;
            int hist[256]={};

            for(n=0;n<N;n++)
            {
                for(l=0;l<N;l++)
                {
                    hist[Imagedata[i+n][j+l]]+=1;
                }
            }
            for(pixel_value=0;pixel_value<=255;pixel_value++)
            {
                if(hist[pixel_value]>max_intensity)
                {
                    max_intensity = hist[pixel_value];
                    intensity=pixel_value;
                }
            }

            output[i][j] = (unsigned char) intensity;
        }
    }

    /////////////////////////////// End ////////////////////////////////////////
    // output image
    if (!(file=fopen("/Users/wenjun/ee569/hw4/comb_optimized_c.raw","wb"))) {
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(output, sizeof(unsigned char), 510*510*1, file);
    fclose(file);
    return 0;
}

