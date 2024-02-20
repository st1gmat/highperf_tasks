#include <mpi.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>

//Посчитать сумму массива чисел. Показать зависимость времени работы программы от
//количества чисел в массиве(10, 1000, 10000000).

int main(int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int num_sizes = 3;
    int sizes[num_sizes] = { 10, 1000, 10000000 };

    for (int s = 0; s < num_sizes; ++s) {
        std::vector<int> array(sizes[s]);
        long long int sum = 0;


        srand(time(nullptr) + rank); // + rank чтобы при новой генерации рандомные числа не повторялись
        for (int i = 0; i < sizes[s]; ++i) {
            array[i] = rand() % 100;
        }

        // подсчет суммы и замер времени
        double start = MPI_Wtime();
        long long int local_sum = 0;
        for (int i = 0; i < sizes[s]; ++i) {
            local_sum += array[i];
        }
        double end = MPI_Wtime();

        std::cout << "process " << rank << ": array size = " << sizes[s] << ", time= " << (end - start) << std::endl;
    }

    MPI_Finalize();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    
    std::cout << "overall time for process " << rank << " : " << elapsed_seconds.count() << "\n";

    return 0;
}