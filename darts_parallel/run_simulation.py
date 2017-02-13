import _darts
import numpy as np
import matplotlib.pyplot as plt
from multiprocessing import Pool
import time

count = 5000000

t0 = time.time()
accuracies = np.arange(0,50,0.0625);
#accuracies = range(50);

def throw(accuracy, target):
    return [accuracy, _darts.darts(count, accuracy, target)]

def throw20(accuracy):
    return throw(accuracy, 20);

def throw1(accuracy):
    return throw(accuracy, 1);

"""
# Multithreaded in python
p = Pool(10);
points20 = zip(*p.map(throw20, accuracies))
points1 = zip(*p.map(throw1, accuracies))
"""

# Multithreaded in c
points20 = zip(*[[a, _darts.darts_parallel(count, a, 20)] for a in accuracies])
points1 = zip(*[[a, _darts.darts_parallel(count, a, 1)] for a in accuracies])
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
