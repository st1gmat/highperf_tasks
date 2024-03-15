#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <vector>

double f(double x, double y) {
    return x * x + y * y * y;
}

int main() {
    const int n = 100000;
    const double h = 0.1;
    std::vector<std::vector<double>> A(n, std::vector<double>(n));
    std::vector<std::vector<double>> B_dx(n, std::vector<double>(n));

    auto start = std::chrono::steady_clock::now();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = f(i * h, j * h);
        }
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == n - 1)
                B_dx[i][j] = 0.0;
            else
                B_dx[i][j] = (A[i + 1][j] - A[i - 1][j]) / (2 * h);
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "time: " << std::chrono::duration<double>(diff).count() << " sec" << std::endl;


    return 0;
}
