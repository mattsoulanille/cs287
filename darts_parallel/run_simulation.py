import _darts
import numpy as np
import matplotlib.pyplot as plt

import time
count = 500000
def throw(accuracy, target):
    return [accuracy, _darts.darts(count, accuracy, target)]

def throw20(accuracy):
    return throw(accuracy, 20)

def throw1(accuracy):
    return throw(accuracy, 1)

# def parallel_throw(accuracy, target):
#     return [accuracy, _darts.darts_parallel(count, accuracy, target)]

# def parallel_throw20(accuracy):
#     return parallel_throw(accuracy, 20)

# def parallel_throw1(accuracy):
#     return parallel_throw(accuracy, 1)
        
        
def run_simulation(parallel_type):
    from multiprocessing import Pool


    t0 = time.time()
    accuracies = np.arange(0,50,0.25)
    #accuracies = range(50)
    p = Pool(10)

    if (parallel_type == 0):
        # Single Threaded
        points20 = zip(*[[a, _darts.darts(count, a, 20)] for a in accuracies])
        points1 = zip(*[[a, _darts.darts(count, a, 1)] for a in accuracies])

    elif (parallel_type == 1):
        # Multithreaded in c
        points20 = zip(*[[a, _darts.darts_parallel(count, a, 20)] for a in accuracies])
        points1 = zip(*[[a, _darts.darts_parallel(count, a, 1)] for a in accuracies])

    elif (parallel_type == 2):
        # Multithreaded in python
        points20 = zip(*p.map(throw20, accuracies))
        points1 = zip(*p.map(throw1, accuracies))

    # elif (parallel_type == 3):
        

    #     print p.map(lambda x:x, accuracies)

    #     points20 = zip(*p.map(parallel_throw20, accuracies))
    #     points1 = zip(*p.map(parallel_throw1, accuracies))
        
    else:
        raise Exception("invalid parallelization type")


    t1 = time.time()
    print("Execution time: " + str(t1-t0))

    # print(points20)
    # print(points1)
    plt.plot(points20[0], points20[1], label="Aiming at 20")
    plt.plot(points1[0], points1[1], label="Aiming at 1")
    plt.legend()
    plt.xlabel("Radius of first standard deviation (in mm)")
    plt.ylabel("Average score")
    plt.title("Monte Carlo Darts")
    plt.show()
    #print(points20)
    #print(points1)



if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='Simulate dart throws.')
    parser.add_argument('--parallelization', '-p', type=int, default=0, help='type of parallelization to use. 0 is none, 1 is c, 2 is python.')
    args = parser.parse_args()

    run_simulation(args.parallelization)
