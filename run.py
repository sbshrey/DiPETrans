import os
import sys
import json
import subprocess
import time

print "experiment started"

cdir = "/scratch/sbshrey/Distributed-Mining-validator"

path = "data/bigquery/simulated/ethereum_data_1_"

logs_path = "logs/simulated/val-new/ethereum_data_1_"


mport = 8090
vport = 10090
vport2 = 11090
mining = 0


#m = 2000

for i in [1,2,4,8,16]:
	for m in [100,200,300,400,500,1000,1500,2000,2500]:
		print "Updating data path"
		data_path = "{}{}_{}.json".format(path,i,m)
		print data_path
	
		#print "Creating Directory"
		dir_path = "{}{}_{}/".format(logs_path,i,m)
		#print dir_path

		print "Running for differnt worker configurations"

		for j in [0,1,2,3,4,5]:
			print
			processes = []
			if j == 0:
				print "serial experiment"
				commands = [
				#"rm -rf {}serial/".format(dir_path),
				"mkdir -p {}serial/".format(dir_path),
				"./serial-contract {} {}serial/ {}".format(data_path, dir_path, mining)
				]

			elif j == 1:
				print "{} worker experiment".format(j)
				node2 = 3
				commands = [
				#"rm -rf {}{}_worker/".format(dir_path,j),
				"mkdir -p {}{}_worker/miner/".format(dir_path,j),
				"mkdir -p {}{}_worker/val/".format(dir_path,j),
				"mkdir -p {}{}_worker/val2/".format(dir_path,j),
				"./masterServer {} {} {} {}{}_worker/miner/ {} &".format(mport, j, data_path, dir_path,j,mining),
				'ssh node{} cd {} && ./masterValidator {} {} {} {}{}_worker/val/ {} & '.format(7,cdir,vport,j,data_path,dir_path,j, vport),
				'ssh node{} cd {} && ./masterValidator {} {} {} {}{}_worker/val2/ {} & '.format(7,cdir,vport2,j,data_path,dir_path,j, vport2),
				'ssh node{} cd {} && ./workerServer {} {} {}{}_worker/miner/ & '.format(node2,cdir,mport+j,j, dir_path,j),
				'ssh node{} cd {} && ./workerServer {} {} {}{}_worker/val/ & '.format(8,cdir,vport+j,j, dir_path,j),
				'ssh node{} cd {} && ./workerServer {} {} {}{}_worker/val2/ & '.format(8,cdir,vport2+j,j, dir_path,j)

				#"./masterClient {}".format(port),
				#"pkill Server"
				]

			else :
				print "{} workers experiment".format(j)

				commands = [
				#"rm -rf {}{}_workers/".format(dir_path,j),
				#"mkdir -p {}{}_workers/".format(dir_path,j),
				#"./masterServer {} {} {} {}{}_workers/ {} &".format(port, j, data_path, dir_path,j,mining),

				"mkdir -p {}{}_workers/miner/".format(dir_path,j),
				"mkdir -p {}{}_workers/val/".format(dir_path,j),
				"mkdir -p {}{}_workers/val2/".format(dir_path,j),
				"./masterServer {} {} {} {}{}_workers/miner/ {} &".format(mport, j, data_path, dir_path,j,mining),
				'ssh node{} cd {} && ./masterValidator {} {} {} {}{}_workers/val/ {} & '.format(7,cdir,vport,j,data_path,dir_path,j, vport),
				'ssh node{} cd {} && ./masterValidator {} {} {} {}{}_workers/val2/ {} & '.format(7,cdir,vport2,j,data_path,dir_path,j, vport2)
				
				]


				nodes = [8,10,11,13,14]
				mnodes = [3,4,5,6,24]
				k = 1
				while k <= j:
				#commands.append("./workerServer {} {} {}{}_workers/ &".format(port+k,k,dir_path,j))
					commands.append('ssh node{} cd {} && ./workerServer {} {} {}{}_workers/miner/ & '.format(mnodes[k-1],cdir,mport+k,k, dir_path,j))
					commands.append('ssh node{} cd {} && ./workerServer {} {} {}{}_workers/val/ & '.format(nodes[k-1],cdir,vport+k,k, dir_path,j))
					commands.append('ssh node{} cd {} && ./workerServer {} {} {}{}_workers/val2/ & '.format(nodes[k-1],cdir,vport2+k,k, dir_path,j))
					k+=1

				#commands.append("./masterClient {}".format(port))
				#commands.append("pkill Server")

			

			for cmd in commands:
				print cmd
				#os.system(cmd)
				p = subprocess.Popen(cmd.split(" "))
				#time.sleep(5)
				if j == 0:
					p.wait()
				else:
					processes.append(p)
		



			if j != 0:
				print "./masterClient", str(mport)
				time.sleep(60)
				p = subprocess.Popen(["./masterClient", str(mport)])
				#print p
				p.wait()
				time.sleep(10)
				os.system("pkill Server")
				for x in processes:
					print "killing", x
					x.kill()

				os.system("./kill.sh")
				#for x in [1,2,3,4,5,6,7,8,10,11,13,14]:
				#	print x
                        	#        os.system("ssh node{} pkill Server".format(x))
				#	os.system("ssh node{} pkill Validator".format(x))
				#	#os.system("ssh node{} pkill Server".format(x))


			time.sleep(10)

