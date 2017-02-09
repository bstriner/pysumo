#include "pysumo.h"

static PyMethodDef PysumoMethods[] = {
{"simulation_start",  pysumo_simulation_start, METH_VARARGS, "Start SUMO"},
{"simulation_stop",  pysumo_simulation_stop, METH_VARARGS, "Stop SUMO"},
{"simulation_step",  pysumo_simulation_step, METH_VARARGS, "Simulate one step"},
{"simulation_minexpectednumber",  pysumo_simulation_minexpectednumber, METH_VARARGS, "Get minmum expected number of vechicles"},
{"inductionloop_meanspeed",  pysumo_inductionloop_meanspeed, METH_VARARGS, "Get induction loop mean speed"},
{"inductionloop_vehiclenumber",  pysumo_inductionloop_vehiclenumber, METH_VARARGS, "Get induction loop vehicle count"},
{"meme_meanspeed",  pysumo_meme_meanspeed, METH_VARARGS, "Get multientry/multiexit mean speed"},
{"meme_vehiclenumber",  pysumo_meme_vehiclenumber, METH_VARARGS, "Get multientry/multiexit vehicle number"},
{"meme_vehiclehaltingnumber",  pysumo_meme_vehiclehaltingnumber, METH_VARARGS, "Get multientry/multiexit vehicle halting number"},
{"meme_vehicleids",  pysumo_meme_vehicleids, METH_VARARGS, "Get multientry/multiexit vehicle IDs"},
{"tls_getstate",  pysumo_tls_getstate, METH_VARARGS, "Get traffic light state"},
{"tls_setstate",  pysumo_tls_setstate, METH_VARARGS, "Set traffic light state"},
{"tls_getControlledLanes", pysumo_tls_getControlledLanes, METH_VARARGS, "get all lanes a traffic light controls"},
{"vehicle_list",  pysumo_vehicle_list, METH_VARARGS, "Get list of vehicles"},
{"vehicle_position",  pysumo_vehicle_position, METH_VARARGS, "Get vehicle position"},
{"vehicle_speed",  pysumo_vehicle_speed, METH_VARARGS, "Get vehicle speed"},
{"vehicle_waitingtime",  pysumo_vehicle_waitingtime, METH_VARARGS, "Get vehicle waiting time"},
{"vehicle_positions",  pysumo_vehicle_positions, METH_VARARGS, "Get list of vehicle positions"},
{"vehicle_speeds",  pysumo_vehicle_speeds, METH_VARARGS, "Get list of vehicle speeds"},
{"vehicle_lane_position", pysumo_vehicle_lane_position, METH_VARARGS, "get relative vehicle position in lane, return a distance from the beginning of the lane"},
{"lane_list", pysumo_lane_list, METH_VARARGS, "get all lanes"},
{"lane_onLaneVehicles",pysumo_lane_onLaneVehicles, METH_VARARGS, "get all vehicles id on a lane"},
{NULL, NULL, 0, NULL}        /* Sentinel */
};

// Initialization function

PyMODINIT_FUNC
initpysumo(void)
{
	(void)Py_InitModule("pysumo", PysumoMethods);
}
