
import camvox
import sys
from math import *

tree = camvox.VoxTree(20.0)
tree.max_depth = 10

s1 = camvox.CSGSphere(1.0)
s1.translate(camvox.Vector(0.0, 0.0, 2.0))
s1.rotate(camvox.Vector(0.0, 1.0, 0.0), -0.25 * pi)

s2 = camvox.CSGSphere(1.0)
s2.translate(camvox.Vector(0.0, 0.0, 4.0))
s2.rotate(camvox.Vector(0.0, 1.0, 0.0), -0.25 * pi)

print >>sys.stderr, "union"
u = camvox.CSGUnion()
u.add(s1)
u.add(s2)
print >>sys.stderr, "rotate"
u.rotate(camvox.Vector(0.0, 1.0, 0.0), -0.25 * pi)
u.translate(camvox.Vector(0.0, 1.0, 1.0))

tree.addCSGObjectOR(u, 1)
print >>sys.stderr, tree.nr_nodes_created, tree.nr_nodes_destroyed, tree.nr_nodes_created - tree.nr_nodes_destroyed
tree.generatePOVCode()

