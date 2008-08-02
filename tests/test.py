
import camvox

tree = camvox.VoxTree()
tree.max_depth = 4
tree.scale = 10.0 / 2147483648.0

s = camvox.CSGSphere()

tree.addCSGObject(s, 0)
tree.generatePOVCode()

