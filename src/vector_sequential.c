#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VECTOR_SIZE 1000000

int main()
{
    int *vector;

    long long total_sum = 0;

    clock_t start_time;
    clock_t end_time;

    double execution_time;

    /* Allocate memory */
    vector = (int *)malloc(VECTOR_SIZE * sizeof(int));

    if (vector == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Create vector: 1, 2, 3, ..., 1000000 */
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        vector[i] = i + 1;
    }

    /* Start timing */
    start_time = clock();

    /* Sequential computation */
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        total_sum += vector[i];
    }

    /* Stop timing */
    end_time = clock();

    /* Calculate execution time */
    execution_time =
        (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\n");
    printf("============================================\n");
    printf("          SEQUENTIAL VECTOR PROCESSING\n");
    printf("============================================\n");

    printf("Vector Size           : %d\n", VECTOR_SIZE);
    printf("Total Sum             : %lld\n", total_sum);
    printf("Execution Time        : %.6f seconds\n", execution_time);

    printf("============================================\n");

    /* Free memory */
    free(vector);

    return 0;
}
