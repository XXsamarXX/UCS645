#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include "correlate.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: ./program <matrix_size>\n";
        return 0;
    }

    int N = std::atoi(argv[1]);

    int ny = N;
    int nx = N;

    std::vector<double> data(ny * nx);
    std::vector<double> result(ny * ny);

    // Initialize matrix
    for (int i = 0; i < ny * nx; i++)
        data[i] = (double)rand() / RAND_MAX;

    // =============================
    // Sequential baseline
    // =============================
    auto start = std::chrono::high_resolution_clock::now();
    correlate_sequential(ny, nx, data.data(), result.data());
    auto end = std::chrono::high_resolution_clock::now();

    double seq_time =
        std::chrono::duration<double>(end - start).count();

    int max_threads = omp_get_max_threads();

    std::cout << "\nMatrix size: " << N << " x " << N << "\n";
    std::cout << "Sequential Time: " << seq_time << " seconds\n\n";

    std::cout << std::setw(10) << "Threads"
              << std::setw(15) << "Time(s)"
              << std::setw(15) << "Speedup"
              << std::setw(15) << "Efficiency"
              << "\n";

    std::cout << "------------------------------------------------------\n";

    // =============================
    // Parallel scaling
    // =============================
    for (int t = 1; t <= max_threads; t++)
    {
        omp_set_num_threads(t);

        auto p_start = std::chrono::high_resolution_clock::now();
        correlate_parallel(ny, nx, data.data(), result.data());
        auto p_end = std::chrono::high_resolution_clock::now();

        double par_time =
            std::chrono::duration<double>(p_end - p_start).count();

        double speedup = seq_time / par_time;
        double efficiency = speedup / t;

        std::cout << std::setw(10) << t
                  << std::setw(15) << par_time
                  << std::setw(15) << speedup
                  << std::setw(15) << efficiency
                  << "\n";
    }

    return 0;
}
