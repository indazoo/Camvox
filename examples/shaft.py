
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
cr.setResolution(0.3)

mat = CappedCylinder(6.0, 50.0)
mat.translate(camvox.Vector(0.0, 0.0, -5.0))
mat.rotate(camvox.Vector(0.0, 1.0, 0.0), 0.5 * pi)
mat.setResolution(0.3)

lastmat = CappedCylinder(2.0, 10.0)
lastmat.translate(camvox.Vector(0.0, 0.0, -25.0))
lastmat.rotate(camvox.Vector(0.0, 1.0, 0.0), 0.5 * pi)
lastmat.setResolution(0.3)

clamp = CappedCylinder(6.0, 10.0)
clamp.translate(camvox.Vector(0.0, 0.0, -35.0))
clamp.rotate(camvox.Vector(0.0, 1.0, 0.0), 0.5 * pi)
clamp.setResolution(0.3)

tree = camvox.VoxTree(90.0)

operation = camvox.VoxOperation()
operation.inside_mask.setLayers(0x01)
operation.inside_op = camvox.VOX_OP_OR;
tree.addCSGObject(clamp, operation)

operation = camvox.VoxOperation()
operation.inside_mask.setLayers(0x02)
operation.inside_op = camvox.VOX_OP_OR;
operation.edge_mask.setLayers(0x04)
operation.edge_op = camvox.VOX_OP_OR;
tree.addCSGObject(cr, operation)

operation = camvox.VoxOperation()
operation.inside_mask.setLayers(0x80)
operation.inside_op = camvox.VOX_OP_OR;
tree.addCSGObject(mat, operation)

operation = camvox.VoxOperation()
operation.inside_mask.setLayers(0x08)
operation.inside_op = camvox.VOX_OP_OR;
tree.addCSGObject(lastmat, operation)


l = camvox.vectorMillCoord()
#print len(l)
tree.getSkin(1, 12.0, l)
#print len(l)

tree.generatePOVCode()
print >>sys.stderr, tree.nr_nodes_created, tree.nr_nodes_destroyed, tree.nr_nodes_created - tree.nr_nodes_destroyed

