
#mpirun --mca btl_tcp_if_include enp2s0 -hostfile QuaCS matrixMPI ./testfiles/Matrix2.txt 100 0
#mpirun --mca btl_tcp_if_include enp2s0 -hostfile QuaCS matrixMPI ./testfiles/e3.txt 3 1
#mpirun --mca btl_tcp_if_include enp2s0 -hostfile QuaCS matrixMPI ./testfiles/e9.txt 9 1
#mpirun --mca btl_tcp_if_include enp2s0 -hostfile QuaCS matrixMPI ./testfiles/Matrix4.txt 10000 0
mpirun -np $1 --mca btl_tcp_if_include enp2s0 --mca plm_rsh_no_tree_spawn 1 -hostfile QuaCS matrixMPI ./testfiles/Matrix4.txt 10000 0
