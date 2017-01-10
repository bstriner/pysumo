
from distutils.core import setup, Extension
import subprocess
import os
import fnmatch

#subprocess.call(['make', '-C', 'src'])
#distutils.command.build
# same dependencies as sumo.exe
extra_objects = [
	'netload/libnetload.a',
	'microsim/libmicrosim.a',
	'microsim/cfmodels/libmicrosimcfmodels.a',
	'microsim/lcmodels/libmicrosimlcmodels.a',
	'microsim/devices/libmicrosimdevs.a',
	'microsim/output/libmicrosimoutput.a',
	'microsim/MSMoveReminder.o',
	'microsim/trigger/libmicrosimtrigger.a',
	'microsim/actions/libmsactions.a',
	'microsim/traffic_lights/libmicrosimtls.a',
	'microsim/pedestrians/libmicrosimpeds.a',
	'mesosim/libmesosim.a',
	'router/librouter.a',
	'traci-server/libtraciserver.a',
	'foreign/tcpip/libtcpip.a',
	'foreign/PHEMlight/cpp/libphemlight.a',
	'utils/common/libcommon.a',
	'utils/distribution/libdistribution.a',
	'utils/importio/libimportio.a',
	'utils/geom/libgeom.a',
	'utils/vehicle/libvehicle.a',
	'utils/shapes/libshapes.a',
	'utils/emissions/libemissions.a',
	'utils/options/liboptions.a',
	'utils/iodevices/libiodevices.a',
	'utils/xml/libxml.a'
]
sumosrc = 'sumo/sumo/src'
#extra_objects = []
#for root, dirnames, filenames in os.walk(sumosrc):
#	for filename in fnmatch.filter(filenames,"lib*.a"):
#		extra_objects.append(os.path.join(root,filename))
#print extra_objects
extra_objects=[os.path.join(sumosrc, o) for o in extra_objects]
# = list(glob.glob("sumo/sumo/src/*.a"))
setup(name='pysumo',
      version='0.0.1',
      description='Pysumo',
      author='Ben Striner',
      author_email='bstriner@gmail.com',
      url='https://github.com/bstriner/pysumo',
      ext_modules=[Extension('pysumo', ['src/pysumo.cpp','sumo/sumo/src/utils/common/IDSupplier.cpp',
	  'sumo/sumo/src/foreign/PHEMlight/cpp/CEP.cpp',
	  'sumo/sumo/src/foreign/PHEMlight/cpp/CEPHandler.cpp'],
	  include_dirs=['sumo/sumo/src', '/usr/include/fox-1.6'],
	  library_dirs=['/usr/lib/x86_64-linux-gnu'],
	  #extra_link_args=["-Wl,--no-undefined"],
	  #'sumo/sumo/libsumo',
	  #runtime_library_dirs =['/usr/lib/x86_64-linux-gnu'],
	  #extra_link_args=["-Lxerces-c-3.1"],
	  libraries=['xerces-c-3.1', 'FOX-1.6', 'proj'],
	  extra_objects=extra_objects
      #packages=['distutils', 'distutils.command'],
     )])
