import _darts
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from multiprocessing import Pool
import time


count = 5000000

workunit_size = 50000
scale = 10
#assert count % workunit_size == 0

workunits = [workunit_size for x in
             range(count // workunit_size)]

if (count % workunit_size > 0):
    workunits.append(count % workunit_size)



def gen_random(count):
    output = []
    for x in range(workunit_size):
        output.append(_darts.randomNormal(scale))
    return output

def test(c):
    return 42


p = Pool(10);
#print p.map(test, [1,2,3])
points_list = p.map(gen_random, workunits)
points = []
for pl in points_list:
    points += pl
#print(workunits)


# instructions from http://matplotlib.org/examples/mplot3d/hist3d_demo.html

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
#x, y = np.random.rand(2, 100) * 4
x, y = zip(*points)
hist, xedges, yedges = np.histogram2d(x, y, bins=32, range=[[-25, 25], [-25, 25]])

xpos, ypos = np.meshgrid(xedges[:-1] + 0.25, yedges[:-1] + 0.25)
xpos = xpos.flatten('F')
ypos = ypos.flatten('F')
zpos = np.zeros_like(xpos)


dx = 0.5 * np.ones_like(zpos)
dy = dx.copy()
dz = hist.flatten()


ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color='b', zsort='average')

plt.show()
