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
    average = sum(trials) / float(len(trials))
    return average

    
def profile_threads():
    threads = [1,2,3,4,5,6,7,8] # Core count to try
    #threads = [1,2,3,4,5] # Core count to try
    trial_count = 50
    darts_thrown = 500000
    
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
    plt.plot(threads, p, 'g-', label="First parallelization method")
    plt.plot(threads, p2, 'r-', label="Second parallelization method")
    plt.legend()
    plt.title("Execution Time for " + str(darts_thrown) + " Darts")

#    v = np.polyfit(threads, p, 2)
#    v2 = np.polyfit(threads, p2, 2)
#    print v
#    plt.plot(threads, v[0] * np.array(threads) ** 2 + v[1]* np.array(threads) + v[2], '-')
    plt.show()

    

def profile_number():
    # Tests with 8 threads

    dart_counts = np.arange(500000, step=5000)
    trial_count = 5
    def test_parallel(darts):
        def test():
            return _darts.darts_parallel(darts, 5, 20, 8)
        return test

    def test_parallel2(darts):
        def test():
            return _darts.darts_parallel2(darts, 5, 20, 8)
        return test


    times = []
    times2 = []

    for c in dart_counts:
        times.append( profile(test_parallel(c), trial_count) )
        times2.append( profile(test_parallel2(c), trial_count) )
    

    plt.xlabel("Number of Darts")
    plt.ylabel("Average Runtime (seconds) from " + str(trial_count) + " trials")
    #plt.xticks(threads)
    plt.plot(dart_counts, times, 'g.', label="First parallelization method")
    plt.plot(dart_counts, times2, 'r.', label="Second parallelization method")
    plt.legend()
    plt.title("Execution time vs Darts Thrown")

    plt.show()



def profile_speedup():

    threads = [1,2,3,4,5,6,7,8] # Core count to try
    #threads = [1,2,3,4,5] # Core count to try
    trial_count = 50
    darts_thrown = 500000

    def test_seq(threads):
        def test():
            return _darts.darts(darts_thrown, 5, 20)
        return test

    
    def test_parallel(threads):
        def test():
            return _darts.darts_parallel(darts_thrown, 5, 20, threads)
        return test

    def test_parallel2(threads):
        def test():
            return _darts.darts_parallel2(darts_thrown, 5, 20, threads)
        return test


    speedups = []
    speedups2 = []

    efficiencies = []
    efficiencies2 = []
    # test how number of threads affects runtime
    for c in threads:
        timeSeq = profile(test_seq(c), trial_count)
        timeP1 = profile(test_parallel(c), trial_count)
        timeP2 = profile(test_parallel2(c), trial_count)
        speedups.append(timeSeq / timeP1)
        speedups2.append(timeSeq / timeP2)

        efficiencies.append(speedups[-1] / c)
        efficiencies2.append(speedups2[-1] / c)

    fig, ax1 = plt.subplots()
    
    plt.xticks(threads)
    plt.xlabel("Number of Threads")
    ax1.set_ylabel("Speedup from " + str(trial_count) + " trials", color='b')
    ax1.plot(threads, speedups, 'b-', label="First parallelization method speedup")
    ax1.plot(threads, speedups2, 'c-', label="Second parallelization method speedup")
    ax1.tick_params('y', colors='b')


    ax2 = ax1.twinx()
    ax2.set_ylabel("Efficiency from " + str(trial_count) + " trials", color='r')
    ax2.plot(threads, efficiencies, 'r-', label="First parallelization method efficiency")
    ax2.plot(threads, efficiencies2, 'm-', label="Second parallelization method efficiency")
    ax2.tick_params('y', colors='r')

    ax1.legend()
    ax2.legend()
    plt.title("Speedup and Efficiency")
    plt.show()

    
if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='Profile dart throwing with different methods of parallelization.')
    parser.add_argument("to_profile", type=str, help="What to profile: speedup, threads, count")
    args = parser.parse_args()
    
    if args.to_profile == 'threads':
        profile_threads()
    elif args.to_profile == 'count':
        profile_number()
    elif args.to_profile == 'speedup':
        profile_speedup()
        
