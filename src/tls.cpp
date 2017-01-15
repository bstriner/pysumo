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
