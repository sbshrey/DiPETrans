import os
import sys
import numpy as np
import matplotlib.pyplot as plt

fig1, ax1 = plt.subplots()
fig2, ax2 = plt.subplots()
ax1.grid(which='both', alpha=0.5)
ax2.grid(which='both', alpha=0.5)


markers = ['-o','-^','-s','-D','-X','-P']

markeredgecolors = ['g','m','c', 'y', 'k']

#ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

font_size = 20

for j in range(0,6):
	txn_time = []
	total_time = []

	if j == 0:
		for i in [1,250,500,1000,2000]:
			if i==1:
				f = open('data/optimized/serial/serial.log'.format(i),'r')
			else:
				f = open('data/optimized/serial/{}/serial.log'.format(i),'r')
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

		ax1.plot(np.arange(1,6), txn_time,markers[j-1],  markerfacecolor='none', markersize = 10, markeredgewidth=2, label="serial")

		print total_time

		ax2.plot(np.arange(1,6), total_time,markers[j-1], markerfacecolor='none',markersize = 10, markeredgewidth=2, label="serial")
	

	else:
		if j == 1:
			worker = "{}_worker".format(j)
		else:
			worker = "{}_workers".format(j)


		for i in [1,250,500,1000,2000]:
			if i==1:
				f = open('data/optimized/distributed/{}/masterClient.log'.format(worker),'r')
			else:
				f = open('data/optimized/distributed/{}/{}/masterClient.log'.format(worker,i),'r')
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

		ax1.plot(np.arange(1,6), txn_time,markers[j-1],  markerfacecolor='none', markersize = 10, markeredgewidth=2, label="{}_worker".format(j))

		print total_time

		ax2.plot(np.arange(1,6), total_time,markers[j-1], markerfacecolor='none',markersize = 10, markeredgewidth=2, label="{}_worker".format(j))
	


ax1.set_xticks(np.arange(1,6))
#ax1.set_xlabel('Configurations')
#ax1.yaxis.grid(which='major', alpha=0.5)
#ax1.tick_params(axis='y', labelcolor='r')
#ax1.set_xticklabels(["1_worker","2_workers","3_workers","4_workers","5_workers"])
ax1.set_ylabel('Transactions processed per seconds', size=font_size)
ax1.set_yticks(np.arange(0,600001,100000))
ax1.set_yticklabels(np.arange(0,600001,100000),size=font_size-5)

#ax1.set_xticks(np.arange(1,6))
ax1.set_xlabel('Transactions per block', size=font_size)

ax1.set_xticklabels(["Original","250","500","1000","2000"],size=font_size-5)
ax1.legend(loc='best',fontsize = font_size)


ax2.set_xticks(np.arange(1,6))


ax2.set_xlabel('Transactions per block', size=font_size)
ax2.set_xticklabels(["Original","250","500","1000","2000"],size=font_size-5)
#ax2.yaxis.grid(which='major', alpha=0.5)
#ax2.yaxis.grid(which='major', alpha=0.5)
#ax2.set_xticklabels(["1_worker","2_workers","3_workers","4_workers","5_workers"])
ax2.set_ylabel('Total time (seconds)',size=font_size)
ax2.tick_params(axis='y')
ax2.set_yticks(np.arange(0,6,1))
ax2.set_yticklabels(np.arange(0,6,1),size=font_size-5)
#fig1.tight_layout()  # otherwise the right y-label is slightly clipped
ax2.legend(loc='best',fontsize = font_size)
plt.show()