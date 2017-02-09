#include "pysumo.h"

PyObject *
pysumo_simulation_start(PyObject *self, PyObject *args)
{
	PyObject * listObj;
	PyObject * strObj;
	int myargc;
	if (! PyArg_ParseTuple( args, "O!", &PyList_Type, &listObj )) return NULL;
	myargc = PyList_Size(listObj);
	char** myargv = new char*[myargc];
	for (int i=0; i<myargc; i++){
		strObj = PyList_GetItem(listObj, i);
		myargv[i] = PyString_AsString( strObj );
	}
	try{
		libsumo_simulation_start(myargc, myargv);
		delete[] myargv;
		Py_RETURN_NONE;
	}
	catch(std::exception ex){
		delete[] myargv;
		PyErr_SetString(PyExc_RuntimeError, ex.what());
		return NULL;
	}
}

PyObject *
pysumo_simulation_stop(PyObject *self, PyObject *args)
{
	PYSUMO_EXCEPTION_START
	libsumo_simulation_stop();
	PYSUMO_EXCEPTION_END
	Py_RETURN_NONE;
}

/**
Make a step in the network
**/

PyObject *
pysumo_simulation_step(PyObject *self, PyObject *args)
{
	PYSUMO_EXCEPTION_START
	libsumo_simulation_step();
	PYSUMO_EXCEPTION_END
	Py_RETURN_NONE;
}

PyObject *
pysumo_simulation_minexpectednumber(PyObject *self, PyObject *args)
{
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("i",libsumo_simulation_minexpectednumber());
	PYSUMO_EXCEPTION_END
}
