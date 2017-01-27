#include <Python.h>
#include "libsumo/libsumo.h"
#include <exception>
#include <stdlib.h>
#include <string>
#include <iostream>


#define PYSUMO_EXCEPTION_START try{
#define PYSUMO_EXCEPTION_END } catch(std::exception ex) \
  {PyErr_SetString(PyExc_RuntimeError, ex.what()); return NULL;}


PyObject *
pysumo_simulation_start(PyObject *self, PyObject *args);
PyObject *
pysumo_simulation_stop(PyObject *self, PyObject *args);
PyObject *
pysumo_simulation_step(PyObject *self, PyObject *args);
PyObject *
pysumo_tls_getstate(PyObject *self, PyObject *args);
PyObject *
pysumo_tls_setstate(PyObject *self, PyObject *args);
PyObject *
pysumo_inductionloop_meanspeed(PyObject *self, PyObject *args);
PyObject *
pysumo_inductionloop_vehiclenumber(PyObject *self, PyObject *args);
PyObject *
pysumo_meme_meanspeed(PyObject *self, PyObject *args);
PyObject *
pysumo_meme_vehiclenumber(PyObject *self, PyObject *args);
PyObject *
pysumo_vehicle_list(PyObject *self, PyObject *args);
PyObject *
pysumo_vehicle_position(PyObject *self, PyObject *args);
PyObject *
pysumo_vehicle_speed(PyObject *self, PyObject *args);
PyObject *
pysumo_vehicle_positions(PyObject *self, PyObject *args);
PyObject *
pysumo_vehicle_speeds(PyObject *self, PyObject *args);
PyObject *
pysumo_vehicle_lane_position(PyObject *self, PyObject *args);