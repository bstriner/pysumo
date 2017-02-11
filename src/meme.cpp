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
	return Py_BuildValue("i", libsumo_meme_vehiclenumber(id));
	PYSUMO_EXCEPTION_END
}

PyObject *
pysumo_meme_vehiclehaltingnumber(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id )) return NULL;
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("i", libsumo_meme_vehiclehaltingnumber(id));
	PYSUMO_EXCEPTION_END
}

PyObject *
pysumo_meme_vehicleids(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id )) return NULL;
	PYSUMO_EXCEPTION_START
 	std::vector<std::string> list = libsumo_meme_vehicleids(id);
        PyObject* ids;
        ids = PyList_New(0);
        for(std::vector<std::string>::iterator it = list.begin(); it!=list.end(); ++it )
        {
            PyList_Append(ids,PyString_FromString((*it).c_str()));
        }
	return ids;
	PYSUMO_EXCEPTION_END
}
