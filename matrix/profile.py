import numpy as np
import matplotlib.pyplot as plt
from time import time
import _matrix


def profile(function, count):
    trials = []
    for i in range(count):
        start = time()
        function()
        end = time()
        trials.append(end - start)
    average = sum(trials) / float(len(trials))
    return average


def profile_threads(matrix):
    threads = [1,2,3,4,5,6,7,8] # Core count to try
    #threads = [1,2,3,4,5] # Core count to try
    trial_count = 2
    size = len(matrix)
    
    def test_parallel(threads):
        def test():
            return _matrix.multiply(matrix, matrix, size, threads)
        return test


    p = []
    # test how number of threads affects runtime
    for c in threads:
        p.append( profile(test_parallel(c), trial_count) )

    plt.xlabel("Number of Threads")
    plt.ylabel("Average Runtime (seconds) from " + str(trial_count) + " trials")
    plt.xticks(threads)
    plt.plot(threads, p, 'g-', label='Parallel')
    plt.legend()
    plt.title("Execution Time for " + str(size) + " by " + str(size) + " Matrix")

    plt.show()


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Profile matrix multiplication.")
    parser.add_argument("matrix_file", type=file, help="File to get NxN matrix from")
    args = parser.parse_args()

    matrix = []
    for line in args.matrix_file.readlines():
#        print [x for x in line.rstrip("\n").split("\t")]
        matrix.append( [float(x) for x in line.rstrip('\n').rstrip('\t').split('\t')] )

    profile_threads(matrix)
