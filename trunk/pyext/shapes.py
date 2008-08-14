
import camvox
import math

_shapes_remember = []

## An infinite xy-plane at the origin, solid is down.
#
def Plane():
	s = camvox.CSGPlane()
	_shapes_remember.append(s)
	return s

## An infinite cylinder at the origin.
# @param r	radius
def Cylinder(r = 0.5):
	s = camvox.CSGCylinder(r)
	_shapes_remember.append(s)
	return s

## A sphere at the origin.
# @param r	radius
def Sphere(r = 0.5):
	s = camvox.CSGSphere(r)
	_shapes_remember.append(s)
	return s

## A union of objects.
# @param shapes	The shapes to add
def Union(*shapes):
	s = camvox.CSGUnion()
	_shapes_remember.append(s)
	for shape in shapes:
		s.add(shape)
	return s

## A intersection of objects.
# @param shapes	The shapes to add
def Intersection(*shapes):
	s = camvox.CSGIntersection()
	_shapes_remember.append(s)
	for shape in shapes:
		s.add(shape)
	return s

## Substract secondary objects from the primary object.
# @param shapes	The shapes to add
def Difference(*shapes):
	s = camvox.CSGDifference()
	_shapes_remember.append(s)
	for shape in shapes:
		s.add(shape)
	return s

## Turns objects inside-out.
# @param shapes	The shapes to add
def Invert(*shapes):
	s = camvox.CSGInvert()
	_shapes_remember.append(s)
	for shape in shapes:
		s.add(shape)
	return s

## A finite solid cylinder.
# @param r	radius of the cylinder
# @param h	hight of the cylinder
def CappedCylinder(r = 0.5, h = 1.0):
	c = Cylinder(r)
	p1 = Plane()
	p1.translate(camvox.Vector(0.0, 0.0, h * 0.5))
	p2 = Plane()
	p2.rotate(camvox.Vector(1.0, 0.0, 0.0), -math.pi)
	p2.translate(camvox.Vector(0.0, 0.0, h * -0.5))
	return Intersection(c, p1, p2)

## A finite solid box.
# @param w	width, length of the x-axis
# @param d	depth, length of the y-axis
# @param h	height, length of the z-axis
def Box(w = 1.0, d = 1.0, h = 1.0):
	p1 = Plane()
	p1.translate(camvox.Vector(0.0, 0.0, h * 0.5))

	p2 = Plane()
	p2.rotate(camvox.Vector(1.0, 0.0, 0.0), -math.pi)
	p2.translate(camvox.Vector(0.0, 0.0, h * -0.5))

	p3 = Plane()
	p3.rotate(camvox.Vector(0.0, 1.0, 0.0), math.pi * 0.5)
	p3.translate(camvox.Vector(w * 0.5, 0.0, 0.0))

	p4 = Plane()
	p4.rotate(camvox.Vector(0.0, 1.0, 0.0), math.pi * -0.5)
	p4.translate(camvox.Vector(w * -0.5, 0.0, 0.0))

	p5 = Plane()
	p5.rotate(camvox.Vector(1.0, 0.0, 0.0), math.pi * -0.5)
	p5.translate(camvox.Vector(0.0, d * 0.5, 0.0))

	p6 = Plane()
	p6.rotate(camvox.Vector(1.0, 0.0, 0.0), math.pi * 0.5)
	p6.translate(camvox.Vector(0.0, d * -0.5, 0.0))

	return Intersection(p1, p2, p3, p4, p5, p6)


