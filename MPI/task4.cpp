#include <iostream>
#include <mpi.h>

using namespace std;

const int N = 10;
const int M = 10;
const int K = 10;

void fillMatrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = rand() % 10;
    }
}

//void printMatrix(int* matrix, int rows, int cols) {
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            cout << matrix[i * cols + j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
//}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = N / size;
    int* A = new int[N * M];
    int* B = new int[M * K];
    int* C = new int[N * K];
    int* local_A = new int[rows_per_process * M];
    int* local_C = new int[rows_per_process * K];

    if (rank == 0) {
        fillMatrix(A, N, M);
        fillMatrix(B, M, K);
    }
    double start = MPI_Wtime();
    MPI_Scatter(A, rows_per_process * M, MPI_INT, local_A, rows_per_process * M, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, M * K, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_process; ++i) {
        for (int j = 0; j < K; ++j) {
            local_C[i * K + j] = 0;
            for (int k = 0; k < M; ++k) {
                local_C[i * K + j] += local_A[i * M + k] * B[k * K + j];
            }
        }
    }

    MPI_Gather(local_C, rows_per_process * K, MPI_INT, C, rows_per_process * K, MPI_INT, 0, MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        //printMatrix(C, N, K);
        cout << N << " x " << M << " time  " << end - start << " sec" << "\n";

    }

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] local_A;
    delete[] local_C;

    MPI_Finalize();
    return 0;
}
