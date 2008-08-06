# used by automake

lib_LTLIBRARIES = 	libcamvox.la
libcamvox_la_CXXFLAGS = -Wall
libcamvox_la_LDFLAGS =  -release 1.0
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
			VoxCoord.cc\
			VoxTree.cc


library_includedir=$(includedir)/camvox-1.0/camvox
library_include_HEADERS=camvox.h\
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
			CSGSPhere.h\
			CSGPlane.h\
			CSGCylinder.h\
			FreeList.h\
			VoxCoord.h\
			VoxTree.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = camvox-1.0.pc


