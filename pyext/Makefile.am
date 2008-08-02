
#INCLUDES = -I/usr/local/boost_1_35_0

BUILT_SOURCES = swig_wrap.cpp
SWIG_SOURCES = swig.i

pkgpython_PYTHON = camvox.py __init__.py
pkgpyexec_LTLIBRARIES = _camvox.la
_camvox_la_SOURCES = $(srcdir)/swig_wrap.cpp $(SWIG_SOURCES)
_camvox_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir) $(BOOST_CPPFLAGS)
_camvox_la_LDFLAGS = -module
_camvox_la_LIBADD = ../camvox/libcamvox.la

$(srcdir)/swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(top_srcdir) -o $@ $<
