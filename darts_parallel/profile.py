import _darts
import numpy as np
import matplotlib.pyplot as plt
from time import time


def profile(function, count):
    trials = []
    for i in range(count):
        start = time()
        function()
        end = time()
        trials.append(end - start)
    average = sum(trials) / len(trials)
    return average

    
def profile_threads():
    threads = [1,2,3,4,5,6,7,8] # Core count to try
    #threads = [1,2,3,4,5] # Core count to try
    trial_count = 5
    darts_thrown = 5000000
    
    def test_parallel(threads):
        def test():
            return _darts.darts_parallel(darts_thrown, 5, 20, threads)
        return test

    def test_parallel2(threads):
        def test():
            return _darts.darts_parallel2(darts_thrown, 5, 20, threads)
        return test


    p = []
    p2 = []
    # test how number of threads affects runtime
    for c in threads:
        p.append( profile(test_parallel(c), trial_count) )
        p2.append( profile(test_parallel2(c), trial_count) )

    # x = np.arange(10)
    # y = 5*x + 10
    # plt.plot(x, y, '.')
    #print p
    #print p2

    plt.xlabel("Number of Threads")
    plt.ylabel("Average Runtime (seconds) from " + str(trial_count) + " trials")
    plt.xticks(threads)
    plt.plot(threads, p, '.', label="First parallelization method")
    plt.plot(threads, p2, '.', label="Second parallelization method")
    plt.legend()
    plt.title("Execution time vs Threads")

#    v = np.polyfit(threads, p, 2)
#    v2 = np.polyfit(threads, p2, 2)
#    print v
#    plt.plot(threads, v[0] * np.array(threads) ** 2 + v[1]* np.array(threads) + v[2], '-')
    plt.show()

    
    
    
    def test_N_parallel(darts):
        def test():
            return _darts.darts_parallel(darts, 5, 20, 8)
        return test

    def test_N_parallel2(darts):
        def test():
            return _darts.darts_parallel2(darts, 5, 20, 8)
        return test




if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='Profile dart throwing with different methods of parallelization.')
    parser.add_argument("to_profile", type=str, help="What to profile: speedup, threads, count")
    args = parser.parse_args()
    
    if args.to_profile == 'threads':
        profile_threads()
