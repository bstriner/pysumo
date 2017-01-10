import pysumo

cmd = ['sumo', 
  '--net-file', 'simple/traffic.net.xml', 
  '--route-files', 'simple/traffic.rou.xml',
  '--additional-files', 'simple/traffic.add.xml',
  '--end', '500']

for i in range(5):
	print "Starting"
	pysumo.start(cmd)
	print "looping"
	for j in range(100):
		pysumo.step()
		#print "Step: {}".format(j)
	pysumo.stop()
