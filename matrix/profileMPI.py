import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from time import time
import os

def profile(function, count):
    trials = []
    for i in range(count):
        start = time()
        function()
        end = time()
        trials.append(end - start)
    average = sum(trials) / float(len(trials))
    return average


def profile_threads():
    size = 1000
    threads = range(1,49)
    #threads = range(1,8,4)
    #threads = [1,2,3,4,5] # Core count to try
    trial_count = 4
    
    def test_parallel(threads):
        def test():
            #return _matrix.multiply(matrix, matrix, size, threads)
            return os.system("mpirun -np " + str(threads) + " --mca btl_tcp_if_include enp2s0 --mca plm_rsh_no_tree_spawn 1 -hostfile QuaCS matrixMPI ./testfiles/Matrix3.txt 1000 0")
        return test


    p = []
    # test how number of threads affects runtime
    for c in threads:
        p.append( profile(test_parallel(c), trial_count) )

    plt.xlabel("Number of Processes")
    plt.ylabel("Average Runtime (seconds) from " + str(trial_count) + " trials")
    #plt.xticks(threads)
    plt.plot(threads, p, 'g-', label='Parallel')
    plt.legend()
    plt.title("Execution Time for " + str(size) + " by " + str(size) + " Matrix")

    plt.savefig('profileMPI.png')
#    plt.show()


if __name__ == "__main__":
#     import argparse
#     parser = argparse.ArgumentParser(description="Profile matrix multiplication.")
#     parser.add_argument("matrix_file", type=file, help="File to get NxN matrix from")
#     args = parser.parse_args()

#     matrix = []
#     for line in args.matrix_file.readlines():
# #        print [x for x in line.rstrip("\n").split("\t")]
#         matrix.append( [float(x) for x in line.rstrip('\n').rstrip('\t').split('\t')] )

    profile_threads()
