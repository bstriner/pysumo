import traci
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
	traci.start(cmd)
	for j in range(1000):
		traci.trafficlights.setRedYellowGreenState("0", random_action())
		traci.simulationStep()
		positions = [traci.vehicle.getPosition(id) for id in traci.vehicle.getIDList()]
	traci.close()
time_end = time()
print "traci time elapsed: {}".format(time_end-time_start)
