#!/bin/bash
#SBATCH --job-name=mergesortseq
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem=10G

make
./mergesort_seq_nocopy 1000000000
