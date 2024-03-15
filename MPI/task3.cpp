#include <iostream>
#include <cmath>
#include <mpi.h>

double f(double x, double y) {
    return sin(x) + cos(y);
}

void compute_derivative(double* A, double* B, int local_rows, int n, double dx) {
    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > 0 && j < n - 1) {
                B[i * n + j] = (A[i * n + j + 1] - A[i * n + j - 1]) / (2 * dx);
            }
            // граничные столбцы устанавливаются в ноль
            else {
                B[i * n + j] = 0.0;
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 10000;
    const double dx = 0.01;

   
    int local_rows = n / size; // количество строк для каждого процесса
    // локальные массивы для входной и выходной матриц
    double* local_A = new double[local_rows * n];
    double* local_B = new double[local_rows * n];
    double* A = new double[n * n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i * n + j] = f(i * dx, j * dx);
        }
    }
    // главный процесс создает глобальную матрицу B
    double* B = nullptr;
    if (rank == 0) {
        B = new double[n * n];
    }

    double start_time = MPI_Wtime();

    if (rank == 0) {
        // главный процесс вычисляет значения функции для всей матрицы A
        
        MPI_Scatter(A, local_rows * n, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD); // разделяем матрицу A между процессами
        delete[] A;
    }
    else {
        
        MPI_Scatter(nullptr, local_rows * n, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD); // процессы, отличные от главного, принимают их сегменты данных
    }
    
    
    compute_derivative(local_A, local_B, local_rows, n, dx); // каждый процесс вычисляет производные для своего сегмента 

    
    MPI_Gather(local_B, local_rows * n, MPI_DOUBLE, B, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD); // процы отправляют свои результаты обратно главному процессу

    double end_time = MPI_Wtime();

    if (rank == 0) {
        /*for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << B[i * n + j] << " ";
            }
            std::cout << std::endl;
        }*/
        std::cout << "time: " << end_time - start_time << " seconds" << std::endl;
    }

    delete[] local_A;
    delete[] local_B;
    if (rank == 0) {
        delete[] B;
    }

    MPI_Finalize();
    return 0;
}
