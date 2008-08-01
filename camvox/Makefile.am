# used by automake
#INCLUDES = -I/usr/local/boost_1_35_0

lib_LTLIBRARIES = 	libcamvox.la
libcamvox_la_CXXFLAGS = -Wall $(BOOST_CPPFLAGS)
libcamvox_la_LDFLAGS =  -release 1.0
libcamvox_la_SOURCES =	CSGObject.cc\
			CSGOperation.cc\
			CSGInvert.cc\
			CSGUnion.cc\
			CSGPrimative.cc\
			CSGSphere.cc\
			VoxCoord.cc\
			VoxNode.cc\
			VoxTree.cc

library_includedir=$(includedir)/camvox-1.0/camvox
library_include_HEADERS=camvox.h\
			Interval.h\
			Vector.h\
			Matrix.h\
			CSGObject.h\
			CSGOperation.h\
			CSGInvert.h\
			CSGUnion.h\
			CSGPrimative.h\
			CSGSPhere.h\
			VoxCoord.h\
			VoxNode.h\
			VoxTree.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = camvox-1.0.pc

bin_PROGRAMS = 		camvox
camvox_CXXFLAGS = -Wall $(BOOST_CPPFLAGS)
camvox_SOURCES =	camvox.cc\
			CSGObject.cc\
			CSGOperation.cc\
			CSGInvert.cc\
			CSGUnion.cc\
			CSGPrimative.cc\
			CSGSphere.cc\
			VoxCoord.cc\
			VoxNode.cc\
			VoxTree.cc


