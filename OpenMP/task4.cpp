#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <random>

void generate_random_matrix(int rows, int cols, std::vector<std::vector<int>>& matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 10);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distribution(gen);
        }
    }
}

void matrix_multiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C) {
    int rows_A = A.size();
    int cols_A = A[0].size();
    int cols_B = B[0].size();
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows_A; ++i) {
        for (int j = 0; j < cols_B; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < cols_A; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    std::vector<std::pair<int, int>> matrix_sizes = { {100, 100}, {500, 500}, {1000, 1000} };

    for (const auto& size : matrix_sizes) {
        int rows_A = size.first;
        int cols_A = size.second;
        int rows_B = cols_A;
        int cols_B = size.second;

        std::vector<std::vector<int>> A(rows_A, std::vector<int>(cols_A));
        std::vector<std::vector<int>> B(rows_B, std::vector<int>(cols_B));
        std::vector<std::vector<int>> C(rows_A, std::vector<int>(cols_B));

        generate_random_matrix(rows_A, cols_A, A);
        generate_random_matrix(rows_B, cols_B, B);

        auto start_time = std::chrono::high_resolution_clock::now();

        // Умножение матриц
        matrix_multiply(A, B, C);

        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end_time - start_time;
        std::cout << "size " << rows_A << "x" << cols_A << " time "<< duration.count() << " sec." << "\n";
    }

    return 0;
}
