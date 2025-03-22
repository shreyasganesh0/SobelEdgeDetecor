#!/bin/bash
#SBATCH --job-name=mat_mul8T1N1600_OMP
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=shreyas.ganesh@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=8
#SBATCH --ntasks-per-socket=4
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 11:00:00
#SBATCH -o mpi_outputs/mat_mul8T1N1600
#0SBATCH -e errors/mat_mul_err_8t1n1600
export OMP_NUM_THREADS=8
srun --mpi=$HPC_PMIX ./mat_mul_mpiomp 1600 1600 1600
