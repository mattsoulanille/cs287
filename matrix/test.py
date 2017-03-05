import _matrix
dim = 1000
v1 = (float(1) for x in range(1000))
v2 = (float(1) for x in range(1000))
print _matrix.dot(v1, v2, dim)

print "\n\n"
with open("testfiles/e3.txt") as i:
    e3 = []
    for row in i:
        e3.append( [float(x) for x in row.rstrip("\n").split("\t")] )


with open("testfiles/e3inv.txt") as i:
    e3inv = []
    for row in i:
        e3inv.append( [float(x) for x in row.rstrip("\n").split("\t")] )

print _matrix.multiply(e3,e3inv,3)
