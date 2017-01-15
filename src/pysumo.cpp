#include "pysumo.h"

static PyMethodDef PysumoMethods[] = {
{"simulation_start",  pysumo_simulation_start, METH_VARARGS, "Start SUMO"},
{"simulation_stop",  pysumo_simulation_stop, METH_VARARGS, "Stop SUMO"},
{"simulation_step",  pysumo_simulation_step, METH_VARARGS, "Simulate one step"},
{"inductionloop_meanspeed",  pysumo_inductionloop_meanspeed, METH_VARARGS, "Get induction loop mean speed"},
{"inductionloop_vehiclenumber",  pysumo_inductionloop_vehiclenumber, METH_VARARGS, "Get induction loop vehicle count"},
{"meme_meanspeed",  pysumo_meme_meanspeed, METH_VARARGS, "Get multientry/multiexit mean speed"},
{"meme_vehiclenumber",  pysumo_meme_vehiclenumber, METH_VARARGS, "Get multientry/multiexit vehicle number"},
{"tls_getstate",  pysumo_tls_getstate, METH_VARARGS, "Get traffic light state"},
{"tls_setstate",  pysumo_tls_setstate, METH_VARARGS, "Set traffic light state"},
{"vehicle_list",  pysumo_vehicle_list, METH_VARARGS, "Get list of vehicles"},
{"vehicle_position",  pysumo_vehicle_position, METH_VARARGS, "Get vehicle position"},
{"vehicle_speed",  pysumo_vehicle_speed, METH_VARARGS, "Get vehicle speed"},
{"vehicle_positions",  pysumo_vehicle_positions, METH_VARARGS, "Get list of vehicle positions"},
{"vehicle_speeds",  pysumo_vehicle_speeds, METH_VARARGS, "Get list of vehicle speeds"},
{NULL, NULL, 0, NULL}        /* Sentinel */
};

// Initialization function

PyMODINIT_FUNC
initpysumo(void)
{
	(void)Py_InitModule("pysumo", PysumoMethods);
}
