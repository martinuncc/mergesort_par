CXXFLAGS=-O3 -fopenmp

mergesort_par: mergesort_par.cpp
	g++ -O3 -fopenmp mergesort_par.cpp -o mergesort_par