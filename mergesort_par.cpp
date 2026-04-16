#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include "tooling/omp_tasking.hpp"
#include <fstream>
using namespace tasking;

#define DEBUG 0

static const size_t PARALLEL_THRESHOLD = 1000;

void generateMergeSortData (std::vector<int>& arr, size_t n) {
    for (size_t  i=0; i< n; ++i) {
        arr[i] = rand();
    }
}
  
void checkMergeSortResult (std::vector<int>& arr, size_t n) {
    bool ok = true;
    for (size_t  i=1; i<n; ++i)
        if (arr[i]< arr[i-1])
            ok = false;
    if(!ok)
        std::cerr<<"notok"<<std::endl;
}

void merge(int* arr, size_t l, size_t mid, size_t r, int* temp) {
    if (l == r) return;
    if (r - l == 1) {
    if (arr[l] > arr[r]) {
        size_t temp = arr[l];
        arr[l] = arr[r];
        arr[r] = temp;
    }
    return;
  }

    size_t n = mid - l;
    
    for (size_t i = 0; i < n; ++i) {
        temp[i] = arr[l + i];
    }

    size_t i = 0, j = mid, k = l;
    while (i < n && j <= r) {
        if (temp[i] <= arr[j]) {
            arr[k++] = temp[i++];
        }
        else {
            arr[k++] = arr[j++];
        }
    }
    while (i < n) {
        arr[k++] = temp[i++];
    }
}

// Sequential merge sort (base case)
void mergesort_seq(int* arr, size_t l, size_t r) {
    if (l >= r) return;
    size_t mid = (l + r) / 2;
    mergesort_seq(arr, l, mid);
    mergesort_seq(arr, mid+1, r);

    size_t n = mid - l + 1;
    std::vector<int> temp(n);
    merge(arr, l, mid+1, r, temp.data());
}

// Parallel merge sort
void mergesort_par(int* arr, size_t l, size_t r) {
    if (l >= r) return;

    size_t len = r - l + 1;
    size_t mid  = (l + r) / 2;

    if (len > PARALLEL_THRESHOLD) {
        taskstart([=]() { mergesort_par(arr, l, mid); });
        taskstart([=]() { mergesort_par(arr, mid+1, r); });

        taskwait();

        size_t n = mid - l + 1;
        std::vector<int> temp(n);
        merge(arr, l, mid+1, r, temp.data());

    } else {
        mergesort_seq(arr, l, r);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <nthreads>" << std::endl;
        return -1;
    }
    std::string filename = "log " + std::string(argv[1]) + "<n> " + std::string(argv[2]) + 
        "<nthreads>" + std::string(argv[2]) + ".txt";
    std::ofstream logFile(filename);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return -1;
    }

    size_t n = atol(argv[1]);
    int nthreads = atoi(argv[2]);

    std::vector<int> arr(n);
    generateMergeSortData(arr, n);

    auto start = std::chrono::high_resolution_clock::now();

    doinparallel([&]() {
        mergesort_par(&arr[0], 0, n - 1);
    }, nthreads);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    

    checkMergeSortResult(arr, n);
    auto elapsed_seconds = std::chrono::duration<double>(end - start);
    std::cerr << duration.count() << std::endl;
    std::cout << "Simulation took " << elapsed_seconds.count() << " seconds." << std::endl;
    std::cout << "Simulation took " << duration.count() << " microseconds." << std::endl;

    logFile << n << " elements and " << nthreads << " threads: " << elapsed_seconds.count() <<" seconds." << std::endl;
    logFile << n << " elements and " << nthreads << " threads: " << duration.count() <<" microseconds." << std::endl;
    logFile.close();
    return 0;
}