import os
import sys
import glob
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm


serial_file = "data/serial/serial.log"
master_server_file = "data/parallel/5_workers/masterServer.log"
master_client_file = "data/parallel/5_workers/masterClient.log"
workers_files = glob.glob('data/parallel/5_workers/workerServer*')           # create the list of file

#masterServer_data = {}
#masterClient_data = {}
#workerServer_data = []


f = open(serial_file,'r')
serial_data = f.readlines()
f.close()

f = open(master_client_file, 'r')
masterClient_data = f.readlines()
f.close()



#end-to-end time and transaction count across blocks

block_numbers = []
end_to_end_time = []
transaction_count = []
uncle_count = []

b_numbers = []



for data in masterClient_data:
	bn,tc,uc,t = data.split("\t")
	block_numbers.append(int(bn))
	end_to_end_time.append(float(t)*1000000)
	transaction_count.append(int(tc))
	if (int(tc) != 0): b_numbers.append(int(bn))
	uncle_count.append(int(uc))

min_y1 = np.min(end_to_end_time)
max_y1 = np.max(end_to_end_time)

print max_y1

min_y2 = np.min(transaction_count)
max_y2 = np.max(transaction_count)

print max_y2


fig1, ax1 = plt.subplots()

color = '#005bd3'
ax1.set_xlabel('Blocks')
ax1.set_ylabel('End-to-End Time (s)', color=color)
ax1.plot(block_numbers, end_to_end_time, color=color,alpha=0.5)
ax1.tick_params(axis='y', labelcolor=color)
ax1.set_yticks(np.arange(0,2001,500))#(max_y1-min_y1)/5))
ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

color = '#c64847'
ax2.set_ylabel('#Transaction/Block', color=color)  # we already handled the x-label with ax1
ax2.plot(block_numbers, transaction_count, color=color,alpha=0.5)
ax2.tick_params(axis='y', labelcolor=color)
ax2.set_yticks(np.arange(0,401,100))
fig1.tight_layout()  # otherwise the right y-label is slightly clipped
#plt.show()


serial_block_numbers = []
serial_end_to_end_time = []
serial_transaction_count = []
serial_uncle_count = []

for data in serial_data:
	bn,tc,uc,t = data.split("\t")
	serial_block_numbers.append(int(bn))
	serial_end_to_end_time.append(float(t)*1000000)
	serial_transaction_count.append(int(tc))
	#if (int(tc) != 0): b_numbers.append(int(bn))
	serial_uncle_count.append(int(uc))


fig2, ax1 = plt.subplots()

#color = '#005bd3'
ax1.set_xlabel('Blocks')
ax1.set_ylabel('End-to-End Time (s)')
ax1.plot(serial_block_numbers, serial_end_to_end_time,label='serial')
ax1.plot(block_numbers, end_to_end_time, label='distributed (workers = 5)')
#ax1.tick_params(axis='y', labelcolor=color)
#ax1.set_yticks(np.arange(0,2001,500))#(max_y1-min_y1)/5))
#plt.legend()
#plt.show()

workerServer_data = {}


#transaction distribution across workers
#fig3, ax1 = plt.subplots()
#ax1.set_xlabel('Blocks')
#ax1.set_ylabel('Transaction count', color=color)


for file_name in workers_files:
	f = open(file_name,"r")
	data = f.readlines()
	f.close()
	tc = []
	total = 0
	for d in data:
		total += int(d.split('\t')[0])
		tc.append(total)
	#workerServer_data[file_name[-5]] = tc
	#print len(b_numbers), len(tc)
	#ax1.plot(b_numbers, tc,label="worker-"+file_name[-5])
	#ax1.tick_params(axis='y', labelcolor=color)


#X, Y = np.meshgrid(x, y)

#CS = ax.contour(X, Y, Z)
#plt.legend()	

#color = '#005bd3'

for file_name in workers_files:

	#fig4, ax1 = plt.subplots()
	#ax1.set_xlabel('Blocks')
	#ax1.set_ylabel('Transaction count', color=color)

	f = open(file_name,"r")
	data = f.readlines()
	f.close()
	tc = []
	for d in data:
		tc.append(int(d.split('\t')[0]))
	#workerServer_data[file_name[-5]] = tc
	#print len(b_numbers), len(tc)
	#ax1.plot(b_numbers, tc,label="worker-"+file_name[-5])


#plt.show()

#analyze data

for i in range(1,6):
	f = open("data/distributed/{}_workers/analyze.csv".format(i),'r')
	data = f.readlines()
	f.close()
	#fig5, ax1 = plt.subplots()
	#ax1.set_xlabel('Blocks')
	#ax1.set_ylabel('Analyze Time (microseconds)')

	b_numbers = []
	analyze_time = []
	f = open("data/distributed/{}_workers/analyze_time.csv".format(i),'w')
	for j in xrange(0,len(data),2):
		bn,t1= data[i].split(',')
		bn,t2 = data[i+1].split(',')
		#if bn1 == bn2:
		t = int(t2)-int(t1)
		f.write(str(t)+"\n")
		b_numbers.append(int(bn))
		analyze_time.append(int(t2)-int(t1))
	f.close()
	#print b_numbers, analyze_time
	#ax1.plot(b_numbers, analyze_time)

#plt.show()
# recvTransactions data

for i in range(1,6):
	for j in range(1,i+1):
		if i==1: 
			f = open("data/distributed/{}_worker/recv_w{}.csv".format(i,j),'r')
			g = open("data/distributed/{}_worker/txn_w{}.csv".format(i,j),'r')
		else: 
			f = open("data/distributed/{}_workers/recv_w{}.csv".format(i,j),'r')
			g = open("data/distributed/{}_workers/txn_w{}.csv".format(i,j),'r')
		
		data = f.readlines()
		data2 = g.readlines()
		
		if i == 1:
			print data2
		
		g.close()
		f.close()
		#fig5, ax1 = plt.subplots()
		#ax1.set_xlabel('Blocks')
		#ax1.set_ylabel('Analyze Time (microseconds)')

		b_numbers = []
		analyze_time = []

		if i==1: 
			f = open("data/distributed/{}_worker/recv_w{}.csv".format(i,j),'w')
			g = open("data/distributed/{}_worker/txn_time_w{}.csv".format(i,j),'w')
			#print g
		else: 
			f = open("data/distributed/{}_workers/recv_time_w{}.csv".format(i,j),'w')
			g = open("data/distributed/{}_workers/txn_time_w{}.csv".format(i,j),'w')

		l = 0
		for k in xrange(0,len(data),2):
			#print data[k]
			bn,t1,w= data[k].split(',')
			bn,t2,w= data[k+1].split(',')
			t = int(t2)-int(t1)
			f.write(str(t)+"\n")

			#if i == 1:
			#	print l
			#	print data2[l].rstrip('\n')
			#if int(data2[l].rstrip('\n')) != 0:
			g.write(str(float(data2[l].rstrip('\n'))/float(t))+"\n")
			#else:
				#print int(data2[l].rstrip('\n'))
			#	pass
			b_numbers.append(int(bn))
			analyze_time.append(int(t2)-int(t1))
			l+=1
		f.close()
		g.close()

# connection to workers data