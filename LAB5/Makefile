CXX = mpic++
CXXFLAGS = -O3

all: daxpy broadcast dot dot primes perfect

daxpy:
	$(CXX) daxpy.cpp -o daxpy

broadcast:
	$(CXX) broadcast.cpp -o broadcast

dot:
	$(CXX) dot_product_mpi.cpp -o dot

primes:
	$(CXX) primes.cpp -o primes

perfect:
	$(CXX) perfect_numbers.cpp -o perfect

clean:
	rm -f daxpy broadcast dot primes perfect