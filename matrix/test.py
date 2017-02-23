import _matrix
v1 = (float(x) for x in range(1000))
v2 = (float(x) for x in range(2000)[::2])
print _matrix.dot(v1, v2)
