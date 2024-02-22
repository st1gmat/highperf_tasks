#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

double calculate_sum(const std::vector<double>& array) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < array.size(); ++i) {
        sum += array[i];
    }
    return sum;
}

int main() {
    std::vector<int> sizes = { 10, 1000, 10000000 };

    for (int size : sizes) {
        std::vector<double> array(size, 1.0);

        auto start = std::chrono::steady_clock::now();
        double sum = calculate_sum(array);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> overall_time = end - start;

        std::cout << "time: " << overall_time.count() << " sec" << "\n";
    }

    return 0;
}