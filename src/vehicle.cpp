#include "pysumo.h"

PyObject *
pysumo_vehicle_list(PyObject *self, PyObject *args)
{
	PyObject* ids;
	ids = PyList_New(0);
	int count = libsumo_vehicle_count();
	std::basic_string<char>* list = new std::basic_string<char>[count];
	libsumo_vehicle_list(list);
	for(int i=0; i<count; i++){
		PyList_Append(ids, PyString_FromString(list[i].c_str()));
	}
	delete [] list;
	return ids;
}

PyObject *
pysumo_vehicle_position(PyObject *self, PyObject *args)
{
	char* id;
	double pos[3];
	if (! PyArg_ParseTuple( args, "s", &id)) return NULL;
	libsumo_vehicle_position(id, pos);
	return Py_BuildValue("ddd",pos[0], pos[1], pos[2]);
}

PyObject *
pysumo_vehicle_speed(PyObject *self, PyObject *args)
{
	char* id;
	if (! PyArg_ParseTuple( args, "s", &id)) return NULL;
	PYSUMO_EXCEPTION_START
	return Py_BuildValue("d", libsumo_vehicle_speed(id));
	PYSUMO_EXCEPTION_END
}

PyObject *
pysumo_vehicle_positions(PyObject *self, PyObject *args)
{
	PyObject* positions;
	positions = PyList_New(0);
	int count = libsumo_vehicle_count();
	double* cpositions = new double[count*3];
	libsumo_vehicle_positions(cpositions);
	for(int i=0; i<count; i++){
		PyList_Append(positions, Py_BuildValue("ddd",
			cpositions[i*3+0],
			cpositions[i*3+1],
			cpositions[i*3+2]));
	}
	delete [] cpositions;
	return positions;
}
PyObject *
pysumo_vehicle_speeds(PyObject *self, PyObject *args)
{
	PyObject* speeds;
	speeds = PyList_New(0);
	int count = libsumo_vehicle_count();
	double* cspeeds = new double[count];
	libsumo_vehicle_speeds(cspeeds);
	for(int i=0; i<count; i++){
		PyList_Append(speeds, Py_BuildValue("d",cspeeds[i]));
	}
	return speeds;
}
