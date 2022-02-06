#include <cmath>
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;
const int THREAD_NUM = 8, N = 5000000;

long diffclock(system_clock::time_point c1, system_clock::time_point c2)
{
    milliseconds ms = duration_cast<milliseconds>(c1 - c2);
    return ms.count();
}

int main()
{
    system_clock::time_point start, end;

    int *a = new int[N];
    int *psum = new int[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = i+1;
    }
    cout << "\n-------- Parallel Prefix Sum ------------\n";
    for (int t = 1; t <= THREAD_NUM; t++)
    {
        for (int i = 0; i < N; i++)
            psum[i] = i+1;

        start = system_clock::now();
        for (int i = 1; i < N; i*=2)
        {
            int temp=N-i;
#pragma omp parallel for num_threads(t) schedule(static, 16)
            for (int j = temp-1; j >= 0; j--)
            {
                psum[i+j] += psum[j];
            }
        }

        end = system_clock::now();
        cout << "number of threads : " << t << " \t time : " << diffclock(end, start) / 1000.0 << endl;
    }
    cout << "----------------------\n\n";
    for(int i=0;i<10;i++)
        cout<<psum[i]<<", ";
    cout<<"...\n";
    delete a,psum;
    return 0;
}