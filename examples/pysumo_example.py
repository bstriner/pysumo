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
	pysumo.start(cmd)
	for j in range(1000):
		pysumo.tls_setstate("0",random_action())
		pysumo.step()	
		positions = pysumo.vehicle_positions()
	pysumo.stop()
time_end = time()
print "pysumo time elapsed: {}".format(time_end-time_start)
