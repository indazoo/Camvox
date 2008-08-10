
import camvox
from camvox.shapes import *
import sys
from math import *

def linkage():	
	b = Box(5.0, 4.0, 2.0)
	b.translate(camvox.Vector(2.5, 0.0, 0.0))

	c = CappedCylinder(2.0, 2.0)
	c.translate(camvox.Vector(5.0, 0.0, 0.0))

	return Union(b, c)

c10 = CappedCylinder(2.0, 10.0)
c10.translate(camvox.Vector(0.0, 0.0, 15.0))

l12 = linkage()
l12.translate(camvox.Vector(0.0, 0.0, 11.0))

c15 = CappedCylinder(1.5, 9.0)
c15.translate(camvox.Vector(5.0, 0.0, 7.5))

l17 = linkage()
l17.translate(camvox.Vector(0.0, 0.0, 4.0))

c20 = CappedCylinder(2.0, 10.0)

l22 = linkage()
l22.rotate(camvox.Vector(0.0, 0.0, 1.0), pi)
l22.translate(camvox.Vector(0.0, 0.0, -4.0))

c25 = CappedCylinder(1.5, 9.0)
c25.translate(camvox.Vector(-5.0, 0.0, -7.5))

l27 = linkage()
l27.rotate(camvox.Vector(0.0, 0.0, 1.0), pi)
l27.translate(camvox.Vector(0.0, 0.0, -11.0))

c30 = CappedCylinder(2.0, 10.0)
c30.translate(camvox.Vector(0.0, 0.0, -15.0))


# Move to center
u = camvox.CSGUnion()
u.add(c10)
u.add(l12)
u.add(c15)
u.add(l17)
u.add(c20)
u.add(l22)
u.add(c25)
u.add(l27)
u.add(c30)
u.translate(camvox.Vector(50.0, 50.0, 50.0))


tree = camvox.VoxTree(100.0)
tree.max_depth = 8

tree.addCSGObjectOR(u, 1)

print >>sys.stderr, tree.nr_nodes_created, tree.nr_nodes_destroyed, tree.nr_nodes_created - tree.nr_nodes_destroyed
tree.generatePOVCode()

