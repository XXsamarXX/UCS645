#include <cmath>
#include <vector>
#include <omp.h>
#include "correlate.h"

// =======================
// SEQUENTIAL VERSION
// =======================
void correlate_sequential(int ny, int nx, const double* data, double* result)
{
    std::vector<double> norm(ny * nx);

    // Normalize rows
    for (int i = 0; i < ny; i++) {

        const double* row = data + i * nx;
        double* norm_row = norm.data() + i * nx;

        double sum = 0.0;
        for (int k = 0; k < nx; k++)
            sum += row[k];

        double mean = sum / nx;

        double sq_sum = 0.0;
        for (int k = 0; k < nx; k++) {
            double val = row[k] - mean;
            norm_row[k] = val;
            sq_sum += val * val;
        }

        double inv_std = 1.0 / std::sqrt(sq_sum);

        for (int k = 0; k < nx; k++)
            norm_row[k] *= inv_std;
    }

    // Correlation
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {

            double dot = 0.0;
            for (int k = 0; k < nx; k++)
                dot += norm[i * nx + k] * norm[j * nx + k];

            result[i * ny + j] = dot;
            result[j * ny + i] = dot; // symmetric
        }
    }
}

// =======================
// PARALLEL VERSION
// =======================
void correlate_parallel(int ny, int nx, const double* data, double* result)
{
    std::vector<double> norm(ny * nx);

    // Parallel normalization
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < ny; i++) {

        const double* row = data + i * nx;
        double* norm_row = norm.data() + i * nx;

        double sum = 0.0;
        for (int k = 0; k < nx; k++)
            sum += row[k];

        double mean = sum / nx;

        double sq_sum = 0.0;
        for (int k = 0; k < nx; k++) {
            double val = row[k] - mean;
            norm_row[k] = val;
            sq_sum += val * val;
        }

        double inv_std = 1.0 / std::sqrt(sq_sum);

        for (int k = 0; k < nx; k++)
            norm_row[k] *= inv_std;
    }

    // Parallel correlation
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < ny; i++) {

        for (int j = 0; j <= i; j++) {

            double dot = 0.0;

            #pragma omp simd reduction(+:dot)
            for (int k = 0; k < nx; k++)
                dot += norm[i * nx + k] * norm[j * nx + k];

            result[i * ny + j] = dot;
            result[j * ny + i] = dot;
        }
    }
}
