
import camvox

tree = camvox.VoxTree()
tree.max_depth = 8
tree.scale = 10.0 / 2147483648.0

s1 = camvox.CSGCylinder(1.0)
s1.translate(camvox.Vector(3.0, 3.0, 3.0))

s2 = camvox.CSGSphere(0.5)
s2.translate(camvox.Vector(3.0, 2.0, 3.0))

u = camvox.CSGIntersection()
u.add(s1)
u.add(s2)

tree.addCSGObject(u, 0)
tree.generatePOVCode()

