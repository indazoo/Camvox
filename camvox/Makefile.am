# used by automake

AM_CXXFLAGS = -I$(srcdir)/..

lib_LTLIBRARIES = 	libcamvox.la
libcamvox_la_CXXFLAGS = -I$(srcdir)/..
libcamvox_la_LDFLAGS =  -release $(VERSION)
libcamvox_la_SOURCES =	CSGObject.cc\
			CSGOperation.cc\
			CSGInvert.cc\
			CSGUnion.cc\
			CSGDifference.cc\
			CSGIntersection.cc\
			CSGPrimative.cc\
			CSGSphere.cc\
			CSGPlane.cc\
			CSGCylinder.cc\
			FreeList.cc\
			VoxTree.cc\
			VoxTree_prune.cc\
			VoxTree_layers.cc\
			VoxTree_addCSGObject.cc\
			VoxTree_generatePOVCode.cc


library_includedir=$(includedir)/camvox-$(VERSION)/camvox
library_include_HEADERS=camvox.h\
			Exceptions.h\
			GMath.h\
			Interval.h\
			Vector.h\
			Matrix.h\
			CSGObject.h\
			CSGOperation.h\
			CSGInvert.h\
			CSGUnion.h\
			CSGDifference.h\
			CSGIntersection.h\
			CSGPrimative.h\
			CSGSphere.h\
			CSGPlane.h\
			CSGCylinder.h\
			FreeList.h\
			Voxel.h\
			VoxOperation.h\
			VoxCoord.h\
			VoxTree.h\
			MillCoord.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = camvox.pc


