#include <stdlib.h>
#include <string.h>
#define uint unsigned int
#define N 101

int f(int m, int M, uint* s)
{
    *s^= (uint)(*s << 13);
    s[0] ^= (uint)(*s >> 17);
    *s ^= (uint)(s[0] << 5);
    return m + (*s%(M-m));
}

void f2(int* a,int* b, int c)
{
        if(c==0 )
                return;
        int t = *b;
        *b = *a;
        *a = t;
}

void f1(int* a)
{
    int t, i, j;
    for(i = 0; i<N; i++)
        for(j=N; j>i; --j)
            f2(&a[j], a+j-1, a[j] < a[j-1]);
}

int median()
{
    uint s;
    int *b=0, i;
    int* a = (int*) malloc(4 * N);
    for(i=0;i<N;i++)
       a[i] = f(-10, 11, &s);
    f1(a);
    *b = a[N/2];
    return *b;
}
