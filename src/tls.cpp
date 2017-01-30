#include "pysumo.h"

PyObject *
pysumo_tls_getstate(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id)) return NULL;	
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("s", libsumo_tls_getstate(id));
	PYSUMO_EXCEPTION_END
}


PyObject *
pysumo_tls_setstate(PyObject *self, PyObject *args)
{
	char* id;
	char* cstate;
	if (! PyArg_ParseTuple( args, "ss", &id, &cstate)) return NULL;
	PYSUMO_EXCEPTION_START
	libsumo_tls_setstate(id, cstate);
	PYSUMO_EXCEPTION_END
	Py_RETURN_NONE;
}

PyObject *
pysumo_tls_getControlledLanes(PyObject *self, PyObject *args)
{
    char* id;
    PyObject* lanes;
    lanes = PyList_New(0);
    if (! PyArg_ParseTuple( args, "s", &id)) return NULL;
    int count = libsumo_tls_controlledLanes_count(id);
    std::basic_string<char>* list = new std::basic_string<char>[count];
    
    libsumo_tls_controlledLanes(id, list);
    for(int i=0; i<count; i++)
    {
        PyList_Append(lanes, PyString_FromString(list[i].c_str()));
    }
    delete[] list;
    return lanes;
    
}
