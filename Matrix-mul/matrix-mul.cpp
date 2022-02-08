#include <cmath>
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;
const int THREAD_NUM = 8, N = 1000;

long diffclock(system_clock::time_point c1, system_clock::time_point c2)
{
    milliseconds ms = duration_cast<milliseconds>(c1 - c2);
    return ms.count();
}

int main()
{
    system_clock::time_point start, end;
    int *a = new int[N * N];
    int *b = new int[N * N];
    int *c = new int[N * N];

    for (int i = 0; i < N * N; i++)
    {
        a[i] = i % 20;
        b[i] = i % 30;
    }
    cout<<"\n-------- Matrix Multiply ------------\n";
    for (int t = 1; t <= THREAD_NUM; t++)
    {
        for (int i = 0; i < N * N; i++)
        {
            c[i] = 0;
        }
        start = system_clock::now();
        for (int i = 0; i < N; i++)
        {
        long long sum = 0;
        int temp=0;
#pragma omp parallel for num_threads(t) schedule(static, 8) reduction(+ : sum) private(temp)
            for (int j = 0; j < N; j++)
            {
                for (int k = 0; k < N; k++)
                {
                    temp= (k+j)%N;
                    sum += a[i * N + temp] * b[temp * N + i];
                }
                #pragma omp atomic write
                    c[i*N+j]=sum;
            }
        }
        end = system_clock::now();
        cout<<"number of threads : "<<t<<" \t time : "<<diffclock(end, start)/1000.0<<endl;
    }
    cout<<"----------------------\n\n";
    delete a,b,c;
    return 0;
}