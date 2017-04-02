#!/bin/bash
#
#SBATCH --job-name=test
#SBATCH --output=res_%j.txt
#
#SBATCH --time=60:00
# SBATCH --nodes=2
#SBATCH --ntasks=33
#SBATCH --ntasks-per-node=17
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=12000
#SBATCH --gres gpu:16

# salloc -t 20

srun ./tc $WORK/data/orkut

