#!/bin/sh
#SBATCH --partition=general-compute
#SBATCH --constraint=CPU-E5645
#SBATCH --time=00:10:00
#SBATCH --nodes=5
#SBATCH --ntasks-per-node=12 
#SBATCH --exclusive 
#SBATCH --job-name="HPC_3_Q2"
#SBATCH --output=pi.out
#SBATCH --mail-user=sbhatt2@buffalo.edu
#SBATCH --mail-type=END
#SBATCH --requeue

SLURM_NPROCS=`srun --nodes=${SLURM_NNODES} bash -c 'hostname' |wc -l`

echo "SLURM_JOBID="$SLURM_JOBID
echo "SLURM_JOB_NODELIST"=$SLURM_JOB_NODELIST
echo "SLURM_NNODES"=$SLURM_NNODES
echo "SLURM_NPROCS"=$SLURM_NTASKs
echo "SLURMTMPDIR="$SLURMTMPDIR
echo "SLURM_NPROCS"=$SLURM_NPROCS

cd $SLURM_SUBMIT_DIR
echo "working directory = "$SLURM_SUBMIT_DIR


module load gcc
module load intel
module unload intel-mpi
module load intel-mpi
module list

ulimit -s unlimited


echo -e "PI using MPI: \n"
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

mpiicpc timer.C pi_MPI_strong.cpp -o pi_s
mpiicpc timer.C pi_MPI_weak.cpp -o pi_w

for((i=1;i<=$SLURM_NPROCS;i++)); do
	echo -e "Result from $i Processes: \n"
	srun -n $i ./pi_s
done
for((j=1;j<=$SLURM_NPROCS;j++)); do
	echo -e "Result from $j Processes: \n"
	srun -n $j ./pi_w
done


echo "Completed!"	
