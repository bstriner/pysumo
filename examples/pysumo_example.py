import pysumo
from tqdm import tqdm
import random
from time import time
cmd = ['sumo', 
  '--net-file', 'simple/traffic.net.xml', 
  '--route-files', 'simple/traffic.rou.xml',
  '--additional-files', 'simple/traffic.add.xml',
  '--end', '500']
actions = ['rGrG','ryry','GrGr','yryr']
def random_action():
	return random.choice(actions)
  
time_start = time()
for i in tqdm(range(500)):
	pysumo.simulation_start(cmd)
#	print 'lanes:', pysumo.tls_getControlledLanes('0');
	print 'all lanes', pysumo.lane_list();
	for j in range(1000):
		pysumo.tls_setstate("0",random_action())
		pysumo.simulation_step()
		ids =  pysumo.lane_onLaneVehicles("0_n_0")
		if ids:
			print ids;
	pysumo.simulation_stop()
time_end = time()
print "pysumo time elapsed: {}".format(time_end-time_start)
