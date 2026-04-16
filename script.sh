#!/bin/bash
#SBATCH --job-name=mergesortp
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem=10G

make

./mergesort_par 500 4
./mergesort_par 1000 4
./mergesort_par 2000 4

./mergesort_par 1000000 2
./mergesort_par 1000000 4
./mergesort_par 1000000 8

./mergesort_par 1000000000 2
./mergesort_par 1000000000 4
./mergesort_par 1000000000 8
