
#mpirun -hostfile QuaCS matrixMPI ./testfiles/Matrix2.txt 100 0
#mpirun -hostfile QuaCS matrixMPI ./testfiles/e3.txt 3 1
#mpirun -hostfile QuaCS matrixMPI ./testfiles/e9.txt 9 1
#mpirun -hostfile QuaCS matrixMPI ./testfiles/Matrix4.txt 10000 0
mpirun -np $1 --mca btl_tcp_if_include enp2s0 -hostfile QuaCS matrixMPI ./testfiles/Matrix3.txt 1000 0
