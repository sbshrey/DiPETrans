import os
import sys
import json
import subprocess
import time

print "experiment started"

path = "data/block16/"

port = 8090



#for i in [1]:
print "Updating data path"
data_path = "{}ethereum_data.json".format(path)
print data_path

#print "Creating Directory"
dir_path = "{}".format(path)
#print dir_path

print "Running for differnt worker configurations"

for j in [0,1,2,3,4,5]:
	print
	processes = []
	if j == 0:
		print "serial experiment"
		commands = [
			"rm -rf {}serial/".format(dir_path),
			"mkdir -p {}serial/".format(dir_path),
			"./serial-mining {} {}serial/ ".format(data_path, dir_path)
		]

	elif j == 1:
		print "{} worker experiment".format(j)
		commands = [
			"rm -rf {}{}_worker/".format(dir_path,j),
			"mkdir -p {}{}_worker/".format(dir_path,j),
			"./masterServer-mining {} {} {} {}{}_worker/ &".format(port, j, data_path, dir_path,j),
			"./workerServer {} {} {}{}_worker/ &".format(port+j,j, dir_path,j)
			#"./masterClient {}".format(port),
			#"pkill Server"
		]

	else :
		print "{} workers experiment".format(j)

		commands = [
			"rm -rf {}{}_workers/".format(dir_path,j),
			"mkdir -p {}{}_workers/".format(dir_path,j),
			"./masterServer-mining {} {} {} {}{}_workers/ &".format(port, j, data_path, dir_path,j),
		]

		k = 1
		while k <= j:
			commands.append("./workerServer {} {} {}{}_workers/ &".format(port+k,k,dir_path,j))
			k+=1

		#commands.append("./masterClient {}".format(port))
		#commands.append("pkill Server")

		

	for cmd in commands:
		print cmd
		#os.system(cmd)
		p = subprocess.Popen(cmd.split(" "))
		if j == 0:
			p.wait()
		else:
			processes.append(p)

	if j != 0:
		print "./masterClient", str(port)
		time.sleep(12)
		p = subprocess.Popen(["./masterClient", str(port)])
		#print p
		p.wait()
		time.sleep(12)
		#os.system("pkill Server")
		for x in processes:
			print "killing", x
			x.kill()
		time.sleep(12)
