#include <xmmintrin.h> //Header for intrinsic functions
#include <stdio.h>
#include <time.h>

int main()
{
    int i, j, k, tito;
    float temp = 0;

    // Variables definition and initialization
    int MAX_DIM = 500;

    float   a[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));
    float   b[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));
    float   c[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));
    float   d[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));

    __m128 row, col, result;


    for(i = 0; i < MAX_DIM; i++)
    {
        for(j =0; j < MAX_DIM; j++)
        {
            a[i][j] = 1.7;

            c[i][j] = 0; // Necessary Initialization - Don't change
            d[i][j] = 0; // Necessary Initialization - Don't change
        }
    }


    // transposing
    for(i = 0; i < MAX_DIM; i++)
    {
        for(j = 0; j < MAX_DIM; j++)
        {
            b[j][i] = a[i][j];
        }
    }


    // unoptimized matrix multiplication
    clock_t time1 = clock();
    for(i = 0; i < MAX_DIM; i++)
    {
        for(j = 0; j < MAX_DIM; j++)
        {
            for(k = 0; k < MAX_DIM; k++)
            {
                c[i][j] += a[i][k] * a[k][j];
            }

            //printf("%d\t", (int) c[i][j]);
        }

        //printf("\n");
    }
    clock_t time2 = clock();


    clock_t time3 = clock();
    
    for(i = 0; i < MAX_DIM; i++)
    {
        for(j = 0; j < MAX_DIM; j++)
        {
            temp = 0;

            for(k = 0; k < MAX_DIM; k+=4)
            {
                
                row = _mm_load_ps(&(a[i][k]));

                col = _mm_load_ps(&(b[j][k]));

                result  = _mm_mul_ps(row, col);
                
                temp += (result[0] + result[1] + result[2] + result[3]);
            }
            
            d[i][j] = temp;
        }
    }

    clock_t time4 = clock();


    // calculate and print execution times
    double TotalTimeLoop = ((double) time2 - (double) time1) / CLOCKS_PER_SEC;
    double TotalTimeSIMD = ((double) time4 - (double) time3) / CLOCKS_PER_SEC;

    printf(" Time taken by loop is %.7f seconds\n", TotalTimeLoop);
    printf(" Time taken by SIMD optimized code is %.7f seconds\n", TotalTimeSIMD);
    printf("Performance gain is x%0.2f\n", TotalTimeLoop/TotalTimeSIMD);

    return 0;
}
