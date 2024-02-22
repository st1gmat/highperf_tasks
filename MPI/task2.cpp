#include <iostream>
#include <vector>
#include <chrono>
#include <mpi.h>


double calculate_partial_sum(const std::vector<double>& array, int start, int end) {
    double sum = 0.0;
    for (int i = start; i < end; ++i)
        sum += array[i];
    return sum;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> sizes = { 1000 }; // размеры массивов для тестирования

    for (int n : sizes) {
        const int part_size = n / size; // размер части массива для каждого процесса
        std::vector<double> array(n);

        if (rank == 0) { // если текущий процесс главный
            srand(time(nullptr));
            for (int i = 0; i < n; ++i) {
                array[i] = 1 + rand() % 100;
            }
            /*std::cout << "full array:" << '\n';
            for (auto i : array)
                std::cout << i << " ";
            std::cout << "\n";*/
        }

        MPI_Bcast(array.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD); // рассылаем массив от главного процесса ко всем остальным P.S. можно было использовать и MPI_Scatter, но так проще имхо

        int start = rank * part_size; // индекс с которого стартует массив 
        int end = (rank >= size - 1) ? n : start + part_size; // конечный индекс для текущего процесса + проверка, чтобы не было приколов с outofrange

        auto start_time = std::chrono::steady_clock::now();
        /*std::cout << " process numer " << rank << "\n";
        for (int i = start; i < end; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << "\n"; */
        double part_sum = calculate_partial_sum(array, start, end); // частичнвая сумма для текущего процесса

        double total_sum;
        
        MPI_Reduce(&part_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); // собираем все в главном процессе, при помощи предопр. функции MPI_SUM

        auto end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> overall_time = end_time - start_time;

        if (rank == 0) {
            std::cout << "time: " << overall_time.count() << " sec\nsize of array: " << n << "\namount of processes: "<< size << "\n";
        }
    }

    MPI_Finalize();

    return 0;
}
