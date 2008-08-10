
import camvox
import sys
from math import *

remember = []

def unit_box():
	p1 = camvox.CSGPlane()
	remember.append(p1)
	p1.translate(camvox.Vector(0.0, 0.0, 0.5))

	p2 = camvox.CSGPlane()
	remember.append(p2)
	p2.rotate(camvox.Vector(1.0, 0.0, 0.0), pi)
	p2.translate(camvox.Vector(0.0, 0.0, -0.5))

	p3 = camvox.CSGPlane()
	remember.append(p3)
	p3.rotate(camvox.Vector(0.0, 1.0, 0.0), -0.5 * pi)
	p3.translate(camvox.Vector(0.5, 0.0, 0.0))

	p4 = camvox.CSGPlane()
	remember.append(p4)
	p4.rotate(camvox.Vector(0.0, 1.0, 0.0), 0.5 * pi)
	p4.translate(camvox.Vector(-0.5, 0.0, 0.0))

	p5 = camvox.CSGPlane()
	remember.append(p5)
	p5.rotate(camvox.Vector(1.0, 0.0, 0.0), -0.5 * pi)
	p5.translate(camvox.Vector(0.0, -0.5, 0.0))

	p6 = camvox.CSGPlane()
	remember.append(p6)
	p6.rotate(camvox.Vector(1.0, 0.0, 0.0), 0.5 * pi)
	p6.translate(camvox.Vector(0.0, 0.5, 0.0))

	i = camvox.CSGIntersection()
	remember.append(i)
	i.add(p1)
	i.add(p2)
	i.add(p3)
	i.add(p4)
	i.add(p5)
	i.add(p6)
	return i

def unit_cylinder():
	c1 = camvox.CSGCylinder(0.5)
	remember.append(c1)

	p1 = camvox.CSGPlane()
	p1.translate(camvox.Vector(0.0, 0.0, 0.5))
	remember.append(p1)

	p2 = camvox.CSGPlane()
	remember.append(p2)
	p2.rotate(camvox.Vector(1.0, 0.0, 0.0), pi)
	p2.translate(camvox.Vector(0.0, 0.0, -0.5))

	u = camvox.CSGIntersection()
	remember.append(u)
	u.add(c1)
	u.add(p1)
	u.add(p2)
	return u

def linkage():	
	b = unit_box()
	b.scale(camvox.Vector(5.0, 4.0, 2.0))
	b.translate(camvox.Vector(2.5, 0.0, 0.0))

	c = unit_cylinder()
	c.scale(camvox.Vector(4.0, 4.0, 2.0))
	c.translate(camvox.Vector(5.0, 0.0, 0.0))

	u = camvox.CSGUnion()
	remember.append(u)
	u.add(b)
	u.add(c)
	return u

c10 = unit_cylinder()
c10.scale(camvox.Vector(4.0, 4.0, 10.0))
c10.translate(camvox.Vector(0.0, 0.0, 15.0))

l12 = linkage()
l12.translate(camvox.Vector(0.0, 0.0, 11.0))

c15 = unit_cylinder()
c15.scale(camvox.Vector(3.0, 3.0, 9.0))
c15.translate(camvox.Vector(5.0, 0.0, 7.5))

l17 = linkage()
l17.translate(camvox.Vector(0.0, 0.0, 4.0))

c20 = unit_cylinder()
c20.scale(camvox.Vector(4.0, 4.0, 10.0))

l22 = linkage()
l22.rotate(camvox.Vector(0.0, 0.0, 1.0), pi)
l22.translate(camvox.Vector(0.0, 0.0, -4.0))

c25 = unit_cylinder()
c25.scale(camvox.Vector(3.0, 3.0, 9.0))
c25.translate(camvox.Vector(-5.0, 0.0, -7.5))

l27 = linkage()
l27.rotate(camvox.Vector(0.0, 0.0, 1.0), pi)
l27.translate(camvox.Vector(0.0, 0.0, -11.0))

c30 = unit_cylinder()
c30.scale(camvox.Vector(4.0, 4.0, 10.0))
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

