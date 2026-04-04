#ifndef CORRELATE_H
#define CORRELATE_H

void correlate_sequential(int ny, int nx, const double* data, double* result);
void correlate_parallel(int ny, int nx, const double* data, double* result);

#endif
