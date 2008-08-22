
import camvox
from camvox.shapes import *
import sys
from math import *

def linkage():	
	b = Box(3.0, 4.0, 2.0)
	b.translate(camvox.Vector(1.5, 0.0, 0.0))

	c = CappedCylinder(2.0, 2.0)
	c.translate(camvox.Vector(3.0, 0.0, 0.0))

	return Union(b, c)

def crank_shaft():
	c10 = CappedCylinder(2.0, 10.0)
	c10.translate(camvox.Vector(0.0, 0.0, 15.0))

	l12 = linkage()
	l12.translate(camvox.Vector(0.0, 0.0, 11.0))

	c15 = CappedCylinder(1.5, 9.0)
	c15.translate(camvox.Vector(3.0, 0.0, 7.5))

	l17 = linkage()
	l17.translate(camvox.Vector(0.0, 0.0, 4.0))

	c20 = CappedCylinder(2.0, 10.0)
	c20.setResolution(0.1)

	l22 = linkage()
	l22.rotate(camvox.Vector(0.0, 0.0, 1.0), pi)
	l22.translate(camvox.Vector(0.0, 0.0, -4.0))

	c25 = CappedCylinder(1.5, 9.0)
	c25.translate(camvox.Vector(-3.0, 0.0, -7.5))

	l27 = linkage()
	l27.rotate(camvox.Vector(0.0, 0.0, 1.0), pi)
	l27.translate(camvox.Vector(0.0, 0.0, -11.0))

	c30 = CappedCylinder(2.0, 10.0)
	c30.translate(camvox.Vector(0.0, 0.0, -15.0))

	return Union(c10, l12, c15, l17, c20, l22, c25, l27, c30)

# Move to center

cr = crank_shaft()
cr.rotate(camvox.Vector(0.0, 1.0, 0.0), 0.5 * pi)
#cr.translate(camvox.Vector(25.0, 10.0, 10.0))
cr.setResolution(0.3)

operation = camvox.VoxOperation()
operation.inside_mask.setLayers(1)
operation.outside_mask.setLayers(0)
operation.edge_mask.setLayers(1)
operation.inside_op = camvox.VOX_OP_NOP;
operation.outside_op = camvox.VOX_OP_NOP;
operation.edge_op = camvox.VOX_OP_OR;

tree = camvox.VoxTree(50.0)
tree.addCSGObject(cr, operation)

print >>sys.stderr, tree.nr_nodes_created, tree.nr_nodes_destroyed, tree.nr_nodes_created - tree.nr_nodes_destroyed
tree.generatePOVCode()

