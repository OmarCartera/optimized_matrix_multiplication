#include <emmintrin.h> //Header for intrinsic functions
#include <stdio.h>
#include <time.h>

int main()
{
    int i, j, k;

    // Variables definition and initialization
    int MAX_DIM = 500;

    double   a[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));
    double   b[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));
    double   c[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));
    double   d[MAX_DIM][MAX_DIM] __attribute__ ((aligned(16)));

    __m128d row, col, result;
    __m128d tempo = _mm_setzero_pd();


    for(i = 0; i < MAX_DIM; i++)
    {
        for(j =0; j < MAX_DIM; j++)
        {
            a[i][j] = 13;

            c[i][j] = 0; // Necessary Initialization - Don't change
            d[i][j] = 0; // Necessary Initialization - Don't change
        }
    }


    clock_t time5 = clock();

    // transposing
    for(i = 0; i < MAX_DIM; i++)
    {
        for(j = 0; j < MAX_DIM; j++)
        {
            b[j][i] = a[i][j];
        }
    }

    clock_t time6 = clock();


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
            for(k = 0; k < MAX_DIM; k+=2)
            {
                
                //row = _mm_load_ps(&(a[i][k]));

                //col = _mm_load_ps(&(b[j][k]));

                //result  = _mm_mul_ps(_mm_load_ps(&(a[i][k])), _mm_load_ps(&(b[j][k])));

                tempo = _mm_add_pd(tempo, _mm_mul_pd(_mm_load_pd(&(a[i][k])), _mm_load_pd(&(b[j][k]))));

                //printf("%.2f %.2f %.2f %.2f\n", tempo[0], tempo[1], tempo[2], tempo[3]);
                
            }
            
            d[i][j] = (tempo[0] + tempo[1]);

            tempo = _mm_setzero_pd();

        }
    }

    clock_t time4 = clock();

    
    for(i = 0; i < MAX_DIM; i++)
    {
        for(j = 0; j < MAX_DIM; j++)
        {
            if(c[i][j] != d[i][j])
                printf("ya5taaaaaaaaaaaaaaaaaaaaay\n");
        }
    }
    


    // calculate and print execution times
    double TotalTimeLoop = ((double) time2 - (double) time1) / CLOCKS_PER_SEC;
    double TotalTimeSIMD = ((double) time4 - (double) time3) / CLOCKS_PER_SEC;
    double TransposeTime = ((double) time6 - (double) time5) / CLOCKS_PER_SEC;

    printf("Time taken by loop is %.7f seconds\n", TotalTimeLoop);
    printf("Time taken by SIMD optimized code is %.7f seconds\n", TotalTimeSIMD);
    printf("Performance gain is x%0.2f\n\n", TotalTimeLoop/TotalTimeSIMD);
    printf("Transpose time is %0.7f seconds\n\n", TransposeTime);

    
    return 0;
}
