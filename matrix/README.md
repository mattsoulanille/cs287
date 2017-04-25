# Matrix

Squares matrices in various multiprocessing ways

## Compiling

```
bash build.sh
```

## Running

### matrix

Single threaded and multithreaded (OpenMP) implementation. Usage: matrix (matrixFile) (dimension) (parallel 0/1) (print 0/1).

Example usage:
```
./matrix testfiles/e3.txt 3 1 1
```

### matrixMPI

Multiprocessor implementation using MPI. See ```run.sh``` for details on running.

Example usage (assumes you're on a haverford quacs lab computer). You will also need to supply the hosts file.
```
./run.sh (number of processors to use)
```

More in-depth usage:
```
mpirun -np 40 --mca btl_tcp_if_include enp2s0 -hostfile QuaCS matrixMPI ./testfiles/Matrix3.txt 1000 0
```

## Profiling

Makes graphs of execution time. You will probably need to generate test matrices. See ```./testfiles/GenMatrix.py``` to generate them.

### profile.py
Profiles parallel execution time for OpenMP

Example usage:
```
python profile.py testfiles/Matrix2.txt
```
### profileMPI

Profiles MPI. Make sure you have the QuaCS host file with all hosts uncommented and that you have your ssh key on each host.
The default matrix size tested is 1000 x 1000, and this can be changed by editing profileMPI.
```
python profileMPI.py
```
