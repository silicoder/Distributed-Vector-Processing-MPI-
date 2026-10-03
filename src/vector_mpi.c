#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_SIZE 1000000

int main(int argc, char *argv[])
{
    int rank, size;
    int elements_per_process;

    int *vector = NULL;
    int *local_vector = NULL;

    long long local_sum = 0;
    long long global_sum = 0;

    int local_min;
    int local_max;
    int global_min;
    int global_max;

    double local_avg;
    double global_avg;

    double start_time, end_time;
    double parallel_time;

    /* Initialize MPI */
    MPI_Init(&argc, &argv);

    /* Get process ID */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Get total number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Check whether vector can be divided equally */
    if (VECTOR_SIZE % size != 0)
    {
        if (rank == 0)
        {
            printf("Error: Vector size %d cannot be evenly divided among %d processes.\n",
                   VECTOR_SIZE, size);
        }

        MPI_Finalize();
        return 1;
    }

    elements_per_process = VECTOR_SIZE / size;

    /* Allocate memory for the complete vector on process 0 */
    if (rank == 0)
    {
        vector = (int *)malloc(VECTOR_SIZE * sizeof(int));

        if (vector == NULL)
        {
            printf("Memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* Create vector: 1, 2, 3, ..., 1000000 */
        for (int i = 0; i < VECTOR_SIZE; i++)
        {
            vector[i] = i + 1;
        }
    }

    /* Allocate memory for each process's portion */
    local_vector = (int *)malloc(elements_per_process * sizeof(int));

    if (local_vector == NULL)
    {
        printf("Process %d: Memory allocation failed.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* Synchronize all processes before timing */
    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    /* Distribute vector among processes */
    MPI_Scatter(
        vector,
        elements_per_process,
        MPI_INT,
        local_vector,
        elements_per_process,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    /* Local computation */
    local_sum = 0;
    local_min = local_vector[0];
    local_max = local_vector[0];

    for (int i = 0; i < elements_per_process; i++)
    {
        local_sum += local_vector[i];

        if (local_vector[i] < local_min)
            local_min = local_vector[i];

        if (local_vector[i] > local_max)
            local_max = local_vector[i];
    }

    local_avg = (double)local_sum / elements_per_process;

    /* Combine local sums */
    MPI_Reduce(
        &local_sum,
        &global_sum,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    /* Find global minimum */
    MPI_Reduce(
        &local_min,
        &global_min,
        1,
        MPI_INT,
        MPI_MIN,
        0,
        MPI_COMM_WORLD
    );

    /* Find global maximum */
    MPI_Reduce(
        &local_max,
        &global_max,
        1,
        MPI_INT,
        MPI_MAX,
        0,
        MPI_COMM_WORLD
    );

    end_time = MPI_Wtime();

    parallel_time = end_time - start_time;

    /* Calculate global average on process 0 */
    if (rank == 0)
    {
        global_avg = (double)global_sum / VECTOR_SIZE;

        printf("\n");
        printf("============================================\n");
        printf("       DISTRIBUTED VECTOR PROCESSING\n");
        printf("============================================\n");

        printf("Vector Size           : %d\n", VECTOR_SIZE);
        printf("MPI Processes         : %d\n", size);
        printf("Elements per Process  : %d\n", elements_per_process);

        printf("============================================\n");

        printf("Process %d -> Elements: %d | Sum: %lld | Min: %d | Max: %d\n",
               rank,
               elements_per_process,
               local_sum,
               local_min,
               local_max);

        printf("\n");
        printf("============================================\n");
        printf("              FINAL RESULTS\n");
        printf("============================================\n");

        printf("Total Sum             : %lld\n", global_sum);
        printf("Global Minimum        : %d\n", global_min);
        printf("Global Maximum        : %d\n", global_max);
        printf("Global Average        : %.2f\n", global_avg);
        printf("Parallel Time         : %.6f seconds\n", parallel_time);

        printf("============================================\n");
    }
    else
    {
        printf("Process %d -> Elements: %d | Sum: %lld | Min: %d | Max: %d\n",
               rank,
               elements_per_process,
               local_sum,
               local_min,
               local_max);
    }

    /* Free memory */
    free(local_vector);

    if (rank == 0)
    {
        free(vector);
    }

    /* Finish MPI */
    MPI_Finalize();

    return 0;
}
