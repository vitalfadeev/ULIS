// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   PyULIS2
*__________________
*
* @file         _PyULIS2_Block.ipp
* @author       Clement Berthaud
* @brief        Python3 module for ULIS2.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <ULIS2>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Types/PyULIS2_PixelValue.ipp"
#include "Types/PyULIS2_PixelProxy.ipp"

/////////////////////////////////////////////////////
/// Object Structure
typedef struct {
    PyObject_HEAD
    ::ul2::FBlock* _mBlock;
} _PyULIS2Object_Block;

/////////////////////////////////////////////////////
/// Dealloc
static void
_PyULIS2Object_Block_dealloc( _PyULIS2Object_Block* self ) {
    delete  self->_mBlock;
    Py_TYPE( self )->tp_free( ( PyObject * ) self );
}

/////////////////////////////////////////////////////
/// Alloc
static PyObject* _PyULIS2Object_Block_new( PyTypeObject* type, PyObject* args, PyObject* kwds )
{
    _PyULIS2Object_Block *self;
    self = (_PyULIS2Object_Block*)type->tp_alloc( type, 0 );
    if (self != NULL) self->_mBlock = nullptr;
    return  (PyObject*)self;
}

/////////////////////////////////////////////////////
/// Init
static int
_PyULIS2Object_Block_init( _PyULIS2Object_Block* self, PyObject* args, PyObject* kwds )
{
    static char *kwlist[] = { "width", "height", "format", NULL };
    int w, h;
    w = h = 0;
    ::ul2::tFormat fmt = 0;

    if( !PyArg_ParseTupleAndKeywords( args, kwds, "iik", kwlist, &w, &h, &fmt ) ) {
        PyErr_SetString( PyExc_TypeError, "Bad input argmuents in init" );
        return -1;
    }

    assert( w > 0 && h > 0 );

    self->_mBlock = new ::ul2::FBlock( w, h, fmt );
    return 0;
}

/////////////////////////////////////////////////////
/// Accessors
#define _PyULIS2Object_Block_GetterMethod_Begin( iName )                                                                \
    static PyObject *                                                                                                   \
    _PyULIS2Object_Block_ ## iName ( _PyULIS2Object_Block* self, PyObject* Py_UNUSED( ignored ) )                       \
    {                                                                                                                   \
        if (self->_mBlock == nullptr) {                                                                                 \
            PyErr_SetString( PyExc_AttributeError, "Bad Access to uninitialized _PyULIS2Object_Block object" );         \
            return NULL;                                                                                                \
        }
#define _PyULIS2Object_Block_GetterMethod_End }

#define _PyULIS2Object_Block_CheckError_SelfNULL if( self->_mBlock == nullptr ) { PyErr_SetString( PyExc_AttributeError, "Bad Access to uninitialized _PyULIS2Object_Block object" ); return NULL; }

// Width
_PyULIS2Object_Block_GetterMethod_Begin( Width              )   return  PyLong_FromLong( self->_mBlock->Width() );              _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( Height             )   return  PyLong_FromLong( self->_mBlock->Height() );             _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( Format             )   return  PyLong_FromLong( self->_mBlock->Format() );             _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( BytesPerSample     )   return  PyLong_FromLong( self->_mBlock->BytesPerSample() );     _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( BytesPerPixel      )   return  PyLong_FromLong( self->_mBlock->BytesPerPixel() );      _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( BytesPerScanLine   )   return  PyLong_FromLong( self->_mBlock->BytesPerScanLine() );   _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( BytesTotal         )   return  PyLong_FromLong( self->_mBlock->BytesTotal() );         _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( Model              )   return  PyLong_FromLong( self->_mBlock->Model() );              _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( Type               )   return  PyLong_FromLong( self->_mBlock->Type() );               _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( HasAlpha           )   return  PyLong_FromLong( self->_mBlock->HasAlpha() );           _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( Swapped            )   return  PyLong_FromLong( self->_mBlock->Swapped() );            _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( Reversed           )   return  PyLong_FromLong( self->_mBlock->Reversed() );           _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( SamplesPerPixel    )   return  PyLong_FromLong( self->_mBlock->SamplesPerPixel() );    _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( NumColorChannels   )   return  PyLong_FromLong( self->_mBlock->NumColorChannels() );   _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( AlphaIndex         )   return  PyLong_FromLong( self->_mBlock->AlphaIndex() );         _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( CRC32              )   return  PyFloat_FromDouble( self->_mBlock->CRC32() );           _PyULIS2Object_Block_GetterMethod_End
_PyULIS2Object_Block_GetterMethod_Begin( MD5                )   return  PyUnicode_FromString( self->_mBlock->MD5().c_str() );   _PyULIS2Object_Block_GetterMethod_End
//_PyULIS2Object_Block_GetterMethod_Begin( UUID               )   return  PyUnicode_FromString( self->_mBlock->UUID().c_str() );  _PyULIS2Object_Block_GetterMethod_End

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Getters
static PyObject*
_PyULIS2Object_Block_PixelProxy( _PyULIS2Object_Block* self, PyObject* args ) {
    _PyULIS2Object_Block_CheckError_SelfNULL
    int x, y;
    if( !PyArg_ParseTuple( args, "ii", &x, &y ) ) return  NULL;

    assert( x >= 0 && x < self->_mBlock->Width() );
    assert( y >= 0 && x < self->_mBlock->Height() );

    PyObject* inst = _PyULIS2Object_PixelProxy_new( &FPixelProxy, nullptr, nullptr );
    _PyULIS2Object_PixelProxy* O = (_PyULIS2Object_PixelProxy*)inst;
    O->super._mPixel = new ::ul2::FPixelProxy( self->_mBlock->PixelProxy( x, y ) );
    return  inst;
}

static PyObject*
_PyULIS2Object_Block_PixelValue( _PyULIS2Object_Block* self, PyObject* args ) {
    _PyULIS2Object_Block_CheckError_SelfNULL
    int x, y;
    if( !PyArg_ParseTuple( args, "ii", &x, &y ) ) return  NULL;

    assert( x >= 0 && x < self->_mBlock->Width() );
    assert( y >= 0 && x < self->_mBlock->Height() );

    PyObject* inst = _PyULIS2Object_PixelValue_new( &FPixelValue, nullptr, nullptr );
    _PyULIS2Object_PixelValue* O = (_PyULIS2Object_PixelValue*)inst;
    O->super._mPixel = new ::ul2::FPixelValue( self->_mBlock->PixelProxy( x, y ) );
    return  inst;
}

/////////////////////////////////////////////////////
/// Meta Methods
static PyMethodDef _PyULIS2Object_Block_methods[] = {
    { "Width"               , (PyCFunction)_PyULIS2Object_Block_Width               , METH_NOARGS, "Width"              },
    { "Height"              , (PyCFunction)_PyULIS2Object_Block_Height              , METH_NOARGS, "Height"             },
    { "Format"              , (PyCFunction)_PyULIS2Object_Block_Format              , METH_NOARGS, "Format"             },
    { "BytesPerSample"      , (PyCFunction)_PyULIS2Object_Block_BytesPerSample      , METH_NOARGS, "BytesPerSample"     },
    { "BytesPerPixel"       , (PyCFunction)_PyULIS2Object_Block_BytesPerPixel       , METH_NOARGS, "BytesPerPixel"      },
    { "BytesPerScanLine"    , (PyCFunction)_PyULIS2Object_Block_BytesPerScanLine    , METH_NOARGS, "BytesPerScanLine"   },
    { "BytesTotal"          , (PyCFunction)_PyULIS2Object_Block_BytesTotal          , METH_NOARGS, "BytesTotal"         },
    { "Model"               , (PyCFunction)_PyULIS2Object_Block_Model               , METH_NOARGS, "Model"              },
    { "Type"                , (PyCFunction)_PyULIS2Object_Block_Type                , METH_NOARGS, "Type"               },
    { "HasAlpha"            , (PyCFunction)_PyULIS2Object_Block_HasAlpha            , METH_NOARGS, "HasAlpha"           },
    { "Swapped"             , (PyCFunction)_PyULIS2Object_Block_Swapped             , METH_NOARGS, "Swapped"            },
    { "Reversed"            , (PyCFunction)_PyULIS2Object_Block_Reversed            , METH_NOARGS, "Reversed"           },
    { "SamplesPerPixel"     , (PyCFunction)_PyULIS2Object_Block_SamplesPerPixel     , METH_NOARGS, "SamplesPerPixel"    },
    { "NumColorChannels"    , (PyCFunction)_PyULIS2Object_Block_NumColorChannels    , METH_NOARGS, "NumColorChannels"   },
    { "AlphaIndex"          , (PyCFunction)_PyULIS2Object_Block_AlphaIndex          , METH_NOARGS, "AlphaIndex"         },
    { "CRC32"               , (PyCFunction)_PyULIS2Object_Block_CRC32               , METH_NOARGS, "CRC32"              },
    { "MD5"                 , (PyCFunction)_PyULIS2Object_Block_MD5                 , METH_NOARGS, "MD5"                },
    //{ "UUID"                , (PyCFunction)_PyULIS2Object_Block_UUID                , METH_NOARGS, "UUID"               },
    { "PixelProxy"          , (PyCFunction)_PyULIS2Object_Block_PixelProxy          , METH_VARARGS, "PixelProxy"        },
    { "PixelValue"          , (PyCFunction)_PyULIS2Object_Block_PixelValue          , METH_VARARGS, "PixelValue"        },
    { NULL } // Sentinel
};

/////////////////////////////////////////////////////
/// Meta Types
static PyTypeObject FBlock = {
PyVarObject_HEAD_INIT(NULL, 0)
    "PyULIS2.FBlock", /* tp_name */
    sizeof( _PyULIS2Object_Block ), /* tp_basicsize */
    0, /* tp_itemsize */
    (destructor)_PyULIS2Object_Block_dealloc, /* tp_dealloc */
    0, /* tp_print */
    0, /* tp_getattr */
    0, /* tp_setattr */
    0, /* tp_as_async */
    0, /* tp_repr */
    0, /* tp_as_number */
    0, /* tp_as_sequence */
    0, /* tp_as_mapping */
    0, /* tp_hash */
    0, /* tp_call */
    0, /* tp_str */
    0, /* tp_getattro */
    0, /* tp_setattro */
    0, /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT, /* tp_flags */
    "ULIS2 Block Object.", /* tp_doc */
    0, /* tp_traverse */
    0, /* tp_clear */
    0, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    0, /* tp_iter */
    0, /* tp_iternext */
    _PyULIS2Object_Block_methods, /* tp_methods */
    0, /* tp_members */
    0, /* tp_getset */
    0, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)_PyULIS2Object_Block_init, /* tp_init */
    0, /* tp_alloc */
    _PyULIS2Object_Block_new, /* tp_new */
    0, /* tp_free */
    0, /* tp_is_gc */
    0, /* tp_bases */
    0, /* tp_mro */
    0, /* tp_cache */
    0, /* tp_subclasses */
    0, /* tp_weaklist */
    0, /* tp_del */
    0, /* tp_version_tag */
    0, /* tp_finalize */
};

