
import camvox

tree = camvox.VoxTree()
tree.max_depth = 8
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

tree.addCSGObject(u, 0)
tree.generatePOVCode()

