import sys
import matplotlib.pyplot as plt


f = open(sys.argv[1], 'r')
data = f.readlines()
f.close()

block = {}
block_number = []
txn_cnt = []

for d in data:
	n,c = d.split(',')
	print n,c
	block[int(n)] = int(c)
	
for b in sorted(block):
	block_number.append(b)
	txn_cnt.append(block[b])



 
plt.plot(block_number, txn_cnt)
#plt.xticks(y_pos, objects)
plt.ylabel('Transactions Count')
plt.xlabel('Block Number')
 
print sum(txn_cnt)
plt.show()
#plt.savefig("plots/blocks_txn_cnt.png")
#plt.show()
