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

static PyObject *
pysumo_testfunc(PyObject *self, PyObject *args)
{
	//const char *command;
	//if (!PyArg_ParseTuple(args, "s", &command))
	//	return NULL;
	return Py_BuildValue("s", "Hello from c++!");
}

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
	char* myargv [250];
	//char*** ret = new char**[numLines];
	for (i=0; i<numLines; i++){
		strObj = PyList_GetItem(listObj, i);
		myargv[i] = PyString_AsString( strObj );
	}
	OptionsIO::setArgs(numLines, myargv);
	//delete ret;
	
    OptionsIO::getOptions();
    if (oc.processMetaOptions(numLines < 2)) {
		SystemFrame::close();
		return NULL;
	}
	XMLSubSys::setValidation(oc.getString("xml-validation"), oc.getString("xml-validation.net"));
	if (!MSFrame::checkOptions()) {
		throw ProcessError();
	}
	MsgHandler::initOutputOptions();
	RandHelper::initRandGlobal();
	RandHelper::initRandGlobal(MSRouteHandler::getParsingRNG());
	// load the net
	gNet = load(oc);
    // preload the routes especially for TraCI
    gNet->loadRoutes();
//	if (net != 0) {
//		ret = net->simulate(string2time(oc.getString("begin")), string2time(oc.getString("end")));
	//} 
	return Py_BuildValue("s", "Hello from c++!");
	//Py_RETURN_NONE
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
	gNet->preSimStepOutput();
	gNet->simulationStep();
	gNet->postSimStepOutput();
	Py_RETURN_NONE;
	 //MSNet::SimulationState state = simulationState(stop)
}

// Method table

static PyMethodDef PysumoMethods[] = {
{"testfunc",  pysumo_testfunc, METH_VARARGS, "My testing function."},
{"start",  pysumo_start, METH_VARARGS, "Start SUMO"},
{"stop",  pysumo_stop, METH_VARARGS, "Stop SUMO"},
{"step",  pysumo_step, METH_VARARGS, "Simulate one step"},
{NULL, NULL, 0, NULL}        /* Sentinel */
};

// Initialization function

PyMODINIT_FUNC
initpysumo(void)
{
	(void)Py_InitModule("pysumo", PysumoMethods);
}
