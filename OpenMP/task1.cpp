#include <iostream>
#include <omp.h>

int main() {

    int num_threads = 4;
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        std::cout << "Privet from " << thread_id << " of " << num_threads << " threads!" << "\n";
    }

    return 0;
}
