
import camvox
import sys
from math import *

tree = camvox.VoxTree()
tree.max_depth = 7
tree.scale = 10.0 / 2147483648.0

s1 = camvox.CSGCylinder(1.0)
s1.translate(camvox.Vector(3.0, 3.0, 3.0))

p1 = camvox.CSGPlane()
p1.translate(camvox.Vector(0.0, 0.0, 3.0))

p2 = camvox.CSGPlane()
p2.translate(camvox.Vector(0.0, 0.0, 1.0))

ip = camvox.CSGInvert()
ip.add(p2)

u = camvox.CSGIntersection()
u.add(s1)
u.add(p1)
u.add(ip)
u.rotate(camvox.Vector(1.0, 0.0, 0.0), 0.25 * pi);

tree.addCSGObjectOR(u, 1)
print >>sys.stderr, tree.nr_nodes_created, tree.nr_nodes_destroyed, tree.nr_nodes_created - tree.nr_nodes_destroyed
tree.generatePOVCode()

