# FUM_MP_ParallelProject-1_1400-1404
Ferdowsi University of Mashhad parallel programming course projects, 1400-1401 Semester 1.

this project applying [Gaussian blur](https://en.wikipedia.org/wiki/Gaussian_blur) filter on input image and using [CImg](https://cimg.eu/) C++ Library for loading and showing the image.

## OpenMP manual (FA)
* [GaussianBlurFilter_OpenMP Manual (FA).pdf](https://github.com/m-hafez/FUM_MP_ParallelProject-1_1400-1401/blob/master/GaussianBlurFilter_OpenMP%20Manual%20(FA).pdf)
## Requirements
* [imagemagick](https://imagemagick.org/script/download.php)

## CUDA version Google Colab
* [FUM_MP_ParallelProject-1_1400-1401.ipynb](https://colab.research.google.com/drive/1SLtdS1veZZU5Fy9zX8lLAZk5o5ogKmJu?usp=sharing)


## compile on windows 
```sh
g++ .\main.cpp -o .\main -O2 -lgdi32
```
* with openmp
```sh
g++ .\main.cpp -o .\main -O2 -lgdi32 -fopenmp
```
## compile on linux

```sh
g++ main.cpp -o main -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 
```
* with openmp
```sh
g++ main.cpp -o main -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -fopenmp
```
* with cuda

 cd to CudaCode directory
```sh
make
```
