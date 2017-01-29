#!/bin/sh
#SBATCH --partition=general-compute
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=12 
#SBATCH --exclusive 
#SBATCH --job-name="HPC_3_Q3"
#SBATCH --output=man.out
#SBATCH --mail-user=sbhatt2@buffalo.edu
#SBATCH --mail-type=END
#SBATCH --requeue
SLURM_NPROCS=`srun --nodes=${SLURM_NNODES} bash -c 'hostname' |wc -l`
echo "SLURM_JOBID="$SLURM_JOBID
echo "SLURM_JOB_NODELIST"=$SLURM_JOB_NODELIST
echo "SLURM_NNODES"=$SLURM_NNODES
echo "SLURM_NPROCS"=$SLURM_NTASKs
echo "SLURMTMPDIR="$SLURMTMPDIR

cd $SLURM_SUBMIT_DIR
echo "working directory = "$SLURM_SUBMIT_DIR


module load intel
module unload intel-mpi
module load intel-mpi
module list

ulimit -s unlimited


echo -e "PI using MPI: \n"
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

mpiicpc timer.C mand_set_mpi.cpp -o mand

for((i=1;i<=$SLURM_NPROCS;i++)); do
	echo -e "\n For $i Processes: \n"
	srun -n $i ./mand
done

 

echo "Completed!"	
