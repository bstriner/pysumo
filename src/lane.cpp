#include "pysumo.h"

PyObject *
pysumo_lane_list(PyObject *self, PyObject *args)
{
    int count = libsumo_lane_count();
    std::basic_string<char>* list = new std::basic_string<char>[count];
    libsumo_lane_list(list);
    PyObject* ids;
	ids = PyList_New(0);
    for( int i=0; i<count; i++)
    {
        PyList_Append(ids, PyString_FromString(list[i].c_str()));
    }
    delete [] list;
	return ids;
}

PyObject *
pysumo_lane_onLaneVehicles(PyObject *self, PyObject *args)
{
        char* id;
        if (! PyArg_ParseTuple( args, "s", &id)) return NULL;
        
        std::vector<std::string> list = libsumo_lane_onLaneVehicles(id);
        PyObject* ids;
        ids = PyList_New(0);
        for(std::vector<std::string>::iterator it = list.begin(); it!=list.end(); ++it )
        {
            PyList_Append(ids,PyString_FromString((*it).c_str()));
        }
        
        return ids;
}
