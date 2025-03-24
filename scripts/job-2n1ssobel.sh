#!/bin/bash
mpicc -qopenmp -o sobel src/hybrid_sobel.c
#SBATCH --job-name=sobel2n1s
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=shreyas.ganesh@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=2
#SBATCH --ntasks-per-socket=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 11:00:00
#SBATCH -o outputs/sobel1N
#0SBATCH -e errors/sobel1n
export OMP_NUM_THREADS=4
srun --mpi=$HPC_PMIX ./sobel > outputs/output_2NS
