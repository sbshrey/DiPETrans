import os
import sys
import numpy as np
import matplotlib.pyplot as plt

fig1, ax1 = plt.subplots()
plt.grid(which='minor', alpha=0.5)
plt.grid(which='major', alpha=0.5)
txn_time = []
total_time = []
for i in range(1,6):
	if i==1:
		f = open('data/distributed/{}_worker/masterClient.log'.format(i),'r')
	else:
		f = open('data/distributed/{}_workers/masterClient.log'.format(i),'r')
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
ax1.set_xticklabels(["1_worker","2_workers","3_workers","4_workers","5_workers"])
ax1.set_ylabel('Transactions processed per seconds')
ax1.set_yticks(np.arange(0,50001,10000))
ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

print total_time

ax2.plot(np.arange(1,6), total_time,'-o', color = 'b', alpha=0.5)
#ax2.yaxis.grid(which='major', alpha=0.5)
#ax2.set_xticklabels(["1_worker","2_workers","3_workers","4_workers","5_workers"])
ax2.set_ylabel('Total time (seconds)')
ax2.set_yticks(np.arange(0,11,2))

plt.show()