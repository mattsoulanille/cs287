import _darts
import numpy as np
import matplotlib.pyplot as plt


count = 500000

accuracies = np.arange(0,50,0.5);
#accuracies = range(50);

points20 = zip(*[[a, _darts.darts(count, a, 20)] for a in accuracies])
points1 = zip(*[[a, _darts.darts(count, a, 1)] for a in accuracies])

# print(points20)
# print(points1)
plt.plot(points20[0], points20[1], label="Aiming at 20")
plt.plot(points1[0], points1[1], label="Aiming at 1")
plt.legend()
plt.xlabel("Standard deviations")
plt.ylabel("Average score")
plt.title("Monte Carlo Darts")
plt.show()
#print(points20)
#print(points1)
