#include "pysumo.h"

PyObject *
pysumo_meme_meanspeed(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id )) return NULL;
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("d", libsumo_meme_meanspeed(id));
	PYSUMO_EXCEPTION_END
}

PyObject *
pysumo_meme_vehiclenumber(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id )) return NULL;
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("i", libsumo_meme_vehiclenumber);
	PYSUMO_EXCEPTION_END
}
