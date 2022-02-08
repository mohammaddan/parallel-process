#ifdef _WIN32
#include "cimg\CImg.h"
#endif
#ifdef linux
#include "cimg/CImg.h"
#endif
#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;
using namespace cimg_library;

size_t num_thrds;
const size_t MAX_THREAD_NUM=8;

double **getGaussian(int height, int width, double sigma)
{
    double **filter;
    double sum = 0.0;
    int i, j;

    filter = new double *[height];
    for (int i = 0; i < height; i++)
        filter[i] = new double[width];

// #pragma omp parallel for num_threads(num_thrds) reduction(+:sum)
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            filter[i][j] = exp(-(i * i + j * j) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            sum += filter[i][j];
        }

// #pragma omp parallel for num_threads(num_thrds) schedule(static, 8)
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            filter[i][j] /= sum;

    return filter;
}

double ***applyFilter(double ***image, double **filter, int width, int height, int filterWidth, int filterHeight)
{

    int newImageHeight = height - filterHeight + 1;
    int newImageWidth = width - filterWidth + 1;
    int d, i, j, h, w;

    double ***newImage;
    newImage = new double **[3];
    for (int i = 0; i < 3; i++)
    {
        newImage[i] = new double *[width];
        for (int j = 0; j < width; j++)
            newImage[i][j] = new double[height];
    }

#pragma omp parallel for num_threads(num_thrds) collapse(2)
    for (d = 0; d < 3; d++)
        for (i = 0; i < newImageWidth; i++)
            for (j = 0; j < newImageHeight; j++)
                for (h = i; h < i + filterWidth; h++)
                    for (w = j; w < j + filterHeight; w++)
                        newImage[d][i][j] += filter[h - i][w - j] * image[d][h][w];

    return newImage;
}

long diffclock(system_clock::time_point c1, system_clock::time_point c2)
{
    milliseconds ms = duration_cast<milliseconds>(c1 - c2);
    return ms.count();
}

int main(int argc, char const *argv[])
{
    system_clock::time_point start, end;

    CImg<unsigned char> originalImage("img/input.jpg");
    double ***kernel,**gaussian;

    int width = originalImage.width();
    int height = originalImage.height();

    kernel = new double **[3];
    for (int i = 0; i < 3; i++)
    {
        kernel[i] = new double *[width];
        for (int j = 0; j < width; j++)
            kernel[i][j] = new double[height];
    }

    //Copy input image to kernel
    for (int k = 0; k < 3; k++)
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                kernel[k][x][y] = originalImage(x, y, 0, k);

    int filterHeight = 25;
    int filterWidth = 25;

    gaussian = getGaussian(filterHeight, filterWidth, 10.0);

    for(num_thrds=1;num_thrds<=MAX_THREAD_NUM;num_thrds++){
        start = system_clock::now();
        kernel = applyFilter(kernel,gaussian, width, height, filterWidth, filterHeight);
        end = system_clock::now();
        cout << "number of threads : " << num_thrds << " \t time : " << diffclock(end, start) / 1000.0 << endl;
    }

    return 0;
}
