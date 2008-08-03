
import camvox

tree = camvox.VoxTree()
tree.max_depth = 8
tree.scale = 10.0 / 2147483648.0

s = camvox.CSGSphere(1.0)
#print s.transform
#print s.total_transform
#print s.total_inv_transform

s.translate(camvox.Vector(3.0, 3.0, 3.0))
s.scale(camvox.Vector(0.2, 0.2, 0.2))
#s.scale(0.5)
#print s.transform
#print s.total_transform
#print s.total_inv_transform

tree.addCSGObject(s, 0)
tree.generatePOVCode()

