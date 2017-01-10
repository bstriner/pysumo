#include <Python.h>
#include <microsim/MSFrame.h>
#include <microsim/output/MSDetectorControl.h>
#include <ctime>
#include <string>
#include <iostream>
#include <microsim/MSGlobals.h>
#include <microsim/MSNet.h>
#include <microsim/MSRoute.h>
#include <microsim/MSVehicleControl.h>
#include <netload/NLBuilder.h>
#include <netload/NLHandler.h>
#include <netload/NLTriggerBuilder.h>
#include <netload/NLEdgeControlBuilder.h>
#include <netload/NLJunctionControlBuilder.h>
#include <netload/NLDetectorBuilder.h>
#include <utils/options/OptionsCont.h>
#include <utils/options/OptionsIO.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/SystemFrame.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/FileHelpers.h>
#include <utils/common/StringTokenizer.h>
#include <utils/common/ToString.h>
#include <utils/xml/XMLSubSys.h>
#include <microsim/MSFrame.h>
#include <microsim/output/MSDetectorControl.h>
#include <utils/iodevices/OutputDevice.h>
#include <mesosim/MEVehicleControl.h>

//#define HAVE_INTERNAL_LANES
//#define NO_TRACI

#ifndef NO_TRACI
#include <traci-server/TraCIServer.h>
#endif

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif

/**

Globals

**/
MSNet* gNet;

/**
 * loads the net, additional routes and the detectors
 */
MSNet*
load(OptionsCont& oc) {
    MSFrame::setMSGlobals(oc);
    MSVehicleControl* vc = 0;
    if (MSGlobals::gUseMesoSim) {
        vc = new MEVehicleControl();
    } else {
        vc = new MSVehicleControl();
    }
    MSNet* net = new MSNet(vc, new MSEventControl(),
                           new MSEventControl(), new MSEventControl());
    NLEdgeControlBuilder eb;
    NLDetectorBuilder db(*net);
    NLJunctionControlBuilder jb(*net, db);
    NLTriggerBuilder tb;
    NLHandler handler("", *net, db, tb, eb, jb);
    tb.setHandler(&handler);
    NLBuilder builder(oc, *net, eb, jb, db, handler);
    if (!builder.build()) {
        delete net;
        throw ProcessError();
    }
    return net;
}

static PyObject *
pysumo_start(PyObject *self, PyObject *args)
{
	OptionsCont& oc = OptionsCont::getOptions();
    oc.setApplicationDescription("A microscopic road traffic simulation.");
    oc.setApplicationName("sumo", "SUMO Version pysumo");
	XMLSubSys::init();
    MSFrame::fillOptions();
	PyObject * listObj;
	PyObject * strObj;
	int numLines;
	int i;
	
	if (! PyArg_ParseTuple( args, "O!", &PyList_Type, &listObj )) return NULL;
	numLines = PyList_Size(listObj);
	char** myargv = new char*[numLines];
	for (i=0; i<numLines; i++){
		strObj = PyList_GetItem(listObj, i);
		myargv[i] = PyString_AsString( strObj );
	}
	OptionsIO::setArgs(numLines, myargv);
	delete[] myargv;
	
    OptionsIO::getOptions();
    if (oc.processMetaOptions(numLines < 2)) {
		SystemFrame::close();
		return NULL;
	}
	XMLSubSys::setValidation(
		oc.getString("xml-validation"), 
		oc.getString("xml-validation.net"));
	if (!MSFrame::checkOptions()) {
		throw ProcessError();
	}
	MsgHandler::initOutputOptions();
	RandHelper::initRandGlobal();
	RandHelper::initRandGlobal(MSRouteHandler::getParsingRNG());
	gNet = load(oc);
    gNet->loadRoutes();
	Py_RETURN_NONE;
}

static PyObject *
pysumo_stop(PyObject *self, PyObject *args)
{
	gNet->closeSimulation(0);
    delete gNet;
    SystemFrame::close();
	Py_RETURN_NONE;
}

/**
Make a step in the network
**/

static PyObject *
pysumo_step(PyObject *self, PyObject *args)
{
	//gNet->preSimStepOutput();
	gNet->simulationStep();
	//gNet->postSimStepOutput();
	Py_RETURN_NONE;
	 //MSNet::SimulationState state = simulationState(stop)
}

static PyObject *
pysumo_inductionloop_meanspeed(PyObject *self, PyObject *args)
{
	char* cname;
	double meanspeed;
	if (! PyArg_ParseTuple( args, "s", &cname )) return NULL;
	MSInductLoop* il = 
		dynamic_cast<MSInductLoop*>(gNet->getDetectorControl()
		.getTypedDetectors(SUMO_TAG_INDUCTION_LOOP).get(cname));
	if (il == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown induction loop");
		return NULL;
	}
	meanspeed =il->getCurrentSpeed();
	return Py_BuildValue("d", meanspeed);
}

static PyObject *
pysumo_inductionloop_vehiclenumber(PyObject *self, PyObject *args)
{
	char* cname;
	int count;
	if (! PyArg_ParseTuple( args, "s", &cname )) return NULL;
	MSInductLoop* il = 
		dynamic_cast<MSInductLoop*>(gNet->getDetectorControl()
		.getTypedDetectors(SUMO_TAG_INDUCTION_LOOP).get(cname));
	if (il == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown induction loop");
		return NULL;
	}
	count =il->getCurrentPassedNumber();
	return Py_BuildValue("i", count);
}

static PyObject *
pysumo_meme_meanspeed(PyObject *self, PyObject *args)
{
	char* cname;
	double meanspeed;
	if (! PyArg_ParseTuple( args, "s", &cname )) return NULL;
	MSE3Collector* e3 = 
		static_cast<MSE3Collector*>(MSNet::getInstance()->getDetectorControl()
		.getTypedDetectors(SUMO_TAG_ENTRY_EXIT_DETECTOR).get(cname));
	if (e3 == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown meme detector");
		return NULL;
	}
	meanspeed = e3->getCurrentMeanSpeed();
	return Py_BuildValue("d", meanspeed);
}

static PyObject *
pysumo_meme_vehiclenumber(PyObject *self, PyObject *args)
{
	char* cname;
	int count;
	if (! PyArg_ParseTuple( args, "s", &cname )) return NULL;
	MSE3Collector* e3 = 
		static_cast<MSE3Collector*>(MSNet::getInstance()->getDetectorControl()
		.getTypedDetectors(SUMO_TAG_ENTRY_EXIT_DETECTOR).get(cname));
	if (e3 == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown meme detector");
		return NULL;
	}
	count = e3->getVehiclesWithin();
	return Py_BuildValue("i", count);
}
static PyObject *
pysumo_tls_getstate(PyObject *self, PyObject *args)
{
	char* cname;
	if (! PyArg_ParseTuple( args, "s", &cname)) return NULL;	
	if (!gNet->getTLSControl().knows(cname)) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown traffic light");
		return NULL;
	}
    MSTLLogicControl::TLSLogicVariants& vars = 
		gNet->getTLSControl().get(cname);
	return Py_BuildValue("s", vars.getActive()->getCurrentPhaseDef().getState());
}


static PyObject *
pysumo_tls_setstate(PyObject *self, PyObject *args)
{
	char* cname;
	char* cstate;
	if (! PyArg_ParseTuple( args, "ss", &cname, &cstate)) return NULL;
	
	if (!gNet->getTLSControl().knows(cname)) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown traffic light");
		return NULL;
	}
    MSTLLogicControl::TLSLogicVariants& vars = 
		gNet->getTLSControl().get(cname);
	vars.setStateInstantiatingOnline(gNet->getTLSControl(), cstate);

	Py_RETURN_NONE;
}

static PyObject *
pysumo_vehicle_list(PyObject *self, PyObject *args)
{
	PyObject* ids;
	ids = PyList_New(0);
	MSVehicleControl& c = gNet->getVehicleControl();
	for (MSVehicleControl::constVehIt i = c.loadedVehBegin();
		i != c.loadedVehEnd(); ++i) {
		if (i->second->isOnRoad() || i->second->isParking()) {
			PyList_Append(ids, PyString_FromString(i->first.c_str()));
		}
	}
	return ids;
}

static PyObject *
pysumo_vehicle_position(PyObject *self, PyObject *args)
{
	char* cname;
	double x,y,z;
	if (! PyArg_ParseTuple( args, "s", &cname)) return NULL;
	
	SUMOVehicle* sumoVehicle = MSNet::getInstance()->getVehicleControl().getVehicle(cname);
	if (sumoVehicle == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown vehicle");
		return NULL;
	}
	MSVehicle* v = dynamic_cast<MSVehicle*>(sumoVehicle);
	if (v == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Vehicle is not a micro-simulation vehicle");
		return NULL;
	}
	bool onRoad = v->isOnRoad();
	bool visible = onRoad || v->isParking();
	
	x=visible ? v->getPosition().x() : INVALID_DOUBLE_VALUE;
	y=visible ? v->getPosition().y() : INVALID_DOUBLE_VALUE;
	z= visible ? v->getPosition().z() : INVALID_DOUBLE_VALUE;
	return Py_BuildValue("ddd",x,y,z);
}

static PyObject *
pysumo_vehicle_speed(PyObject *self, PyObject *args)
{
	char* cname;
	double speed;
	if (! PyArg_ParseTuple( args, "s", &cname)) return NULL;
	
	SUMOVehicle* sumoVehicle = MSNet::getInstance()->getVehicleControl().getVehicle(cname);
	if (sumoVehicle == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown vehicle");
		return NULL;
	}
	MSVehicle* v = dynamic_cast<MSVehicle*>(sumoVehicle);
	if (v == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Vehicle is not a micro-simulation vehicle");
		return NULL;
	}
	bool onRoad = v->isOnRoad();
	bool visible = onRoad || v->isParking();
	speed = visible ? v->getSpeed() : INVALID_DOUBLE_VALUE;
	return Py_BuildValue("d",speed);
}

static PyObject *
pysumo_vehicle_positions(PyObject *self, PyObject *args)
{
	PyObject* positions;
	positions = PyList_New(0);
	MSVehicleControl& c = gNet->getVehicleControl();
	for (MSVehicleControl::constVehIt i = c.loadedVehBegin();
		i != c.loadedVehEnd(); ++i) {
		if (i->second->isOnRoad() || i->second->isParking()) {
			SUMOVehicle* sumoVehicle = c.getVehicle(i->first.c_str());
			if (sumoVehicle == 0) {
				PyErr_SetString(PyExc_RuntimeError, "Unknown vehicle");
				return NULL;
			}
			MSVehicle* v = dynamic_cast<MSVehicle*>(sumoVehicle);
			if (v == 0) {
				PyErr_SetString(PyExc_RuntimeError, "Vehicle is not a micro-simulation vehicle");
				return NULL;
			}
			bool onRoad = v->isOnRoad();
			bool visible = onRoad || v->isParking();
			double x=visible ? v->getPosition().x() : INVALID_DOUBLE_VALUE;
			double y=visible ? v->getPosition().y() : INVALID_DOUBLE_VALUE;
			double z= visible ? v->getPosition().z() : INVALID_DOUBLE_VALUE;
			PyList_Append(positions, Py_BuildValue("ddd",x,y,z));
		}
	}
	return positions;
}
static PyObject *
pysumo_vehicle_speeds(PyObject *self, PyObject *args)
{
	PyObject* speeds;
	speeds = PyList_New(0);
	MSVehicleControl& c = gNet->getVehicleControl();
	for (MSVehicleControl::constVehIt i = c.loadedVehBegin();
		i != c.loadedVehEnd(); ++i) {
		if (i->second->isOnRoad() || i->second->isParking()) {
			SUMOVehicle* sumoVehicle = c.getVehicle(i->first.c_str());
			if (sumoVehicle == 0) {
				PyErr_SetString(PyExc_RuntimeError, "Unknown vehicle");
				return NULL;
			}
			MSVehicle* v = dynamic_cast<MSVehicle*>(sumoVehicle);
			if (v == 0) {
				PyErr_SetString(PyExc_RuntimeError, "Vehicle is not a micro-simulation vehicle");
				return NULL;
			}
			bool onRoad = v->isOnRoad();
			bool visible = onRoad || v->isParking();
			double speed = visible ? v->getSpeed() : INVALID_DOUBLE_VALUE;
			PyList_Append(speeds, Py_BuildValue("d",speed));
		}
	}
	return speeds;
}

// Method table

static PyMethodDef PysumoMethods[] = {
{"start",  pysumo_start, METH_VARARGS, "Start SUMO"},
{"stop",  pysumo_stop, METH_VARARGS, "Stop SUMO"},
{"step",  pysumo_step, METH_VARARGS, "Simulate one step"},
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
