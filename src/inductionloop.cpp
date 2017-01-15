#include "pysumo.h"

PyObject *
pysumo_inductionloop_meanspeed(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id )) return NULL;
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("d", libsumo_inductionloop_meanspeed(id));
	PYSUMO_EXCEPTION_END
}

PyObject *
pysumo_inductionloop_vehiclenumber(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id )) return NULL;
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("i", libsumo_inductionloop_vehiclenumber);
	PYSUMO_EXCEPTION_END
}
