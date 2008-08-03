# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.36
#
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _camvox
import new
new_instancemethod = new.instancemethod
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


class Interval(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Interval, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Interval, name)
    __repr__ = _swig_repr
    __swig_setmethods__["low"] = _camvox.Interval_low_set
    __swig_getmethods__["low"] = _camvox.Interval_low_get
    if _newclass:low = _swig_property(_camvox.Interval_low_get, _camvox.Interval_low_set)
    __swig_setmethods__["high"] = _camvox.Interval_high_set
    __swig_getmethods__["high"] = _camvox.Interval_high_get
    if _newclass:high = _swig_property(_camvox.Interval_high_get, _camvox.Interval_high_set)
    def __init__(self, *args): 
        this = _camvox.new_Interval(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_Interval
    __del__ = lambda self : None;
Interval_swigregister = _camvox.Interval_swigregister
Interval_swigregister(Interval)

class Vector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vector, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_Vector(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_Vector
    __del__ = lambda self : None;
Vector_swigregister = _camvox.Vector_swigregister
Vector_swigregister(Vector)

class Matrix(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Matrix, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Matrix, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_Matrix(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_Matrix
    __del__ = lambda self : None;
    def __str__(*args): return _camvox.Matrix___str__(*args)
Matrix_swigregister = _camvox.Matrix_swigregister
Matrix_swigregister(Matrix)

class CSGObject(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGObject, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSGObject, name)
    __repr__ = _swig_repr
    __swig_setmethods__["transform"] = _camvox.CSGObject_transform_set
    __swig_getmethods__["transform"] = _camvox.CSGObject_transform_get
    if _newclass:transform = _swig_property(_camvox.CSGObject_transform_get, _camvox.CSGObject_transform_set)
    __swig_setmethods__["total_transform"] = _camvox.CSGObject_total_transform_set
    __swig_getmethods__["total_transform"] = _camvox.CSGObject_total_transform_get
    if _newclass:total_transform = _swig_property(_camvox.CSGObject_total_transform_get, _camvox.CSGObject_total_transform_set)
    __swig_setmethods__["total_inv_transform"] = _camvox.CSGObject_total_inv_transform_set
    __swig_getmethods__["total_inv_transform"] = _camvox.CSGObject_total_inv_transform_get
    if _newclass:total_inv_transform = _swig_property(_camvox.CSGObject_total_inv_transform_get, _camvox.CSGObject_total_inv_transform_set)
    def __init__(self, *args): 
        this = _camvox.new_CSGObject(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGObject
    __del__ = lambda self : None;
    def translate(*args): return _camvox.CSGObject_translate(*args)
    def scale(*args): return _camvox.CSGObject_scale(*args)
CSGObject_swigregister = _camvox.CSGObject_swigregister
CSGObject_swigregister(CSGObject)

class CSGPrimative(CSGObject):
    __swig_setmethods__ = {}
    for _s in [CSGObject]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGPrimative, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGObject]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGPrimative, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGPrimative(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGPrimative
    __del__ = lambda self : None;
CSGPrimative_swigregister = _camvox.CSGPrimative_swigregister
CSGPrimative_swigregister(CSGPrimative)

class CSGSphere(CSGPrimative):
    __swig_setmethods__ = {}
    for _s in [CSGPrimative]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGSphere, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGPrimative]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGSphere, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGSphere(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGSphere
    __del__ = lambda self : None;
CSGSphere_swigregister = _camvox.CSGSphere_swigregister
CSGSphere_swigregister(CSGSphere)

class CSGCylinder(CSGPrimative):
    __swig_setmethods__ = {}
    for _s in [CSGPrimative]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGCylinder, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGPrimative]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGCylinder, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGCylinder(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGCylinder
    __del__ = lambda self : None;
CSGCylinder_swigregister = _camvox.CSGCylinder_swigregister
CSGCylinder_swigregister(CSGCylinder)

class CSGOperation(CSGObject):
    __swig_setmethods__ = {}
    for _s in [CSGObject]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGOperation, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGObject]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGOperation, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGOperation(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGOperation
    __del__ = lambda self : None;
    def add(*args): return _camvox.CSGOperation_add(*args)
CSGOperation_swigregister = _camvox.CSGOperation_swigregister
CSGOperation_swigregister(CSGOperation)

class CSGUnion(CSGOperation):
    __swig_setmethods__ = {}
    for _s in [CSGOperation]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGUnion, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGOperation]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGUnion, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGUnion(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGUnion
    __del__ = lambda self : None;
CSGUnion_swigregister = _camvox.CSGUnion_swigregister
CSGUnion_swigregister(CSGUnion)

class CSGDifference(CSGOperation):
    __swig_setmethods__ = {}
    for _s in [CSGOperation]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGDifference, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGOperation]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGDifference, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGDifference(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGDifference
    __del__ = lambda self : None;
CSGDifference_swigregister = _camvox.CSGDifference_swigregister
CSGDifference_swigregister(CSGDifference)

class CSGInvert(CSGOperation):
    __swig_setmethods__ = {}
    for _s in [CSGOperation]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSGInvert, name, value)
    __swig_getmethods__ = {}
    for _s in [CSGOperation]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CSGInvert, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_CSGInvert(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_CSGInvert
    __del__ = lambda self : None;
CSGInvert_swigregister = _camvox.CSGInvert_swigregister
CSGInvert_swigregister(CSGInvert)

class VoxCoord(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VoxCoord, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VoxCoord, name)
    __repr__ = _swig_repr
    __swig_setmethods__["v"] = _camvox.VoxCoord_v_set
    __swig_getmethods__["v"] = _camvox.VoxCoord_v_get
    if _newclass:v = _swig_property(_camvox.VoxCoord_v_get, _camvox.VoxCoord_v_set)
    __swig_setmethods__["depth"] = _camvox.VoxCoord_depth_set
    __swig_getmethods__["depth"] = _camvox.VoxCoord_depth_get
    if _newclass:depth = _swig_property(_camvox.VoxCoord_depth_get, _camvox.VoxCoord_depth_set)
    def __init__(self, *args): 
        this = _camvox.new_VoxCoord(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_VoxCoord
    __del__ = lambda self : None;
    def nextNeighbour(*args): return _camvox.VoxCoord_nextNeighbour(*args)
    def childCoord(*args): return _camvox.VoxCoord_childCoord(*args)
    def boundingBox(*args): return _camvox.VoxCoord_boundingBox(*args)
VoxCoord_swigregister = _camvox.VoxCoord_swigregister
VoxCoord_swigregister(VoxCoord)

class VoxTree(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VoxTree, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VoxTree, name)
    __repr__ = _swig_repr
    __swig_setmethods__["max_depth"] = _camvox.VoxTree_max_depth_set
    __swig_getmethods__["max_depth"] = _camvox.VoxTree_max_depth_get
    if _newclass:max_depth = _swig_property(_camvox.VoxTree_max_depth_get, _camvox.VoxTree_max_depth_set)
    __swig_setmethods__["nr_nodes_created"] = _camvox.VoxTree_nr_nodes_created_set
    __swig_getmethods__["nr_nodes_created"] = _camvox.VoxTree_nr_nodes_created_get
    if _newclass:nr_nodes_created = _swig_property(_camvox.VoxTree_nr_nodes_created_get, _camvox.VoxTree_nr_nodes_created_set)
    __swig_setmethods__["nr_nodes_destroyed"] = _camvox.VoxTree_nr_nodes_destroyed_set
    __swig_getmethods__["nr_nodes_destroyed"] = _camvox.VoxTree_nr_nodes_destroyed_get
    if _newclass:nr_nodes_destroyed = _swig_property(_camvox.VoxTree_nr_nodes_destroyed_get, _camvox.VoxTree_nr_nodes_destroyed_set)
    __swig_setmethods__["scale"] = _camvox.VoxTree_scale_set
    __swig_getmethods__["scale"] = _camvox.VoxTree_scale_get
    if _newclass:scale = _swig_property(_camvox.VoxTree_scale_get, _camvox.VoxTree_scale_set)
    def __init__(self, *args): 
        this = _camvox.new_VoxTree(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_VoxTree
    __del__ = lambda self : None;
    def generatePOVCode(*args): return _camvox.VoxTree_generatePOVCode(*args)
    def addCSGObject(*args): return _camvox.VoxTree_addCSGObject(*args)
    def prune(*args): return _camvox.VoxTree_prune(*args)
VoxTree_swigregister = _camvox.VoxTree_swigregister
VoxTree_swigregister(VoxTree)

class VoxNode(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VoxNode, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VoxNode, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _camvox.new_VoxNode(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _camvox.delete_VoxNode
    __del__ = lambda self : None;
    def prune(*args): return _camvox.VoxNode_prune(*args)
    def addCSGObject(*args): return _camvox.VoxNode_addCSGObject(*args)
    def generatePOVCode(*args): return _camvox.VoxNode_generatePOVCode(*args)
VoxNode_swigregister = _camvox.VoxNode_swigregister
VoxNode_swigregister(VoxNode)



