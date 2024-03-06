#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

double f(double x, double y) {
    return x * x + y * y * y;
}

int main() {
    const int N = 10;
    const double h = 0.1; // расстояние между соседними точками на сетке
    double A[N][N]; // сетка для значений функции f(x, y)
    double B_dx[N][N]; // сетка для значений производной df/dx
    double B_dy[N][N]; // сетка для значений производной df/dy

    // Замер времени начала выполнения
    auto start = std::chrono::steady_clock::now();

    // заполнение сетки A значениями функции f(x, y)
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = f(i * h, j * h);
        }
    }

    // вычисление частных производных на сетке
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N - 1) // граничное условие, тк если мы на краю сетки, то у нас нет соседних точек в заданном направлении
                B_dx[i][j] = 0.0;
            else
                B_dx[i][j] = (A[i + 1][j] - A[i - 1][j]) / (2 * h); // формула центральной разностной схемы для x

            if (j == 0 || j == N - 1)
                B_dy[i][j] = 0.0;
            else
                B_dy[i][j] = (A[i][j + 1] - A[i][j - 1]) / (2 * h); // аналогично, но для y
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "time: " << std::chrono::duration<double>(diff).count() << " sec" << std::endl;

    std::cout << "f(x, y):\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << A[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "\ndf/dx:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << B_dx[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "\ndf/dy:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << B_dy[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
