import os
import sys
import numpy as np
import matplotlib.pyplot as plt

fig1, ax1 = plt.subplots()
plt.grid(which='minor', alpha=0.5)
plt.grid(which='major', alpha=0.5)
txn_time = []
total_time = []
for i in [1,250,500,1000,2000]:
	if i==1:
		f = open('data/serial/serial.log'.format(i),'r')
	else:
		f = open('data/serial/{}/serial.log'.format(i),'r')
	data = f.readlines()
	f.close()

	transaction_count = []
	time = []

	for d in data:
		bn,tc,uc,t = d.split('\t')
		transaction_count.append(int(tc))
		time.append(float(t)/1000000)

	total_time.append(sum(time))
	txn_time.append(float(sum(transaction_count))/sum(time))

ax1.plot(np.arange(1,6), txn_time,'-o', color = 'r', alpha=0.5)
ax1.set_xlabel('Configurations')
ax1.set_xticks(np.arange(1,6))
ax1.yaxis.grid(which='major', alpha=0.5)
ax1.tick_params(axis='y', labelcolor='r')
ax1.set_xticklabels(["Original","250/block","500/block","1000/block","2000/block"])
ax1.set_ylabel('Transactions processed per seconds',color='r')
ax1.set_yticks(np.arange(0,180001,30000))
ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

print total_time

ax2.plot(np.arange(1,6), total_time,'-o', color = 'b', alpha=0.5)
#ax2.yaxis.grid(which='major', alpha=0.5)
#ax2.set_xticklabels(["1_worker","2_workers","3_workers","4_workers","5_workers"])
ax2.set_ylabel('Total time (seconds)',color='b')
ax2.tick_params(axis='y', labelcolor='b')
ax2.set_yticks(np.arange(0,3.1,0.5))
fig1.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()