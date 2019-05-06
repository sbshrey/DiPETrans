awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/1_worker/workerServer0.log > data/distributed/1_worker/time_txn_w1.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/2_workers/workerServer0.log > data/distributed/2_workers/time_txn_w1.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/2_workers/workerServer1.log > data/distributed/2_workers/time_txn_w2.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/3_workers/workerServer0.log > data/distributed/3_workers/time_txn_w1.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/3_workers/workerServer1.log > data/distributed/3_workers/time_txn_w2.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/3_workers/workerServer2.log > data/distributed/3_workers/time_txn_w3.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/4_workers/workerServer0.log > data/distributed/4_workers/time_txn_w1.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/4_workers/workerServer1.log > data/distributed/4_workers/time_txn_w2.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/4_workers/workerServer2.log > data/distributed/4_workers/time_txn_w3.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/4_workers/workerServer3.log > data/distributed/4_workers/time_txn_w4.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/5_workers/workerServer0.log > data/distributed/5_workers/time_txn_w1.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/5_workers/workerServer1.log > data/distributed/5_workers/time_txn_w2.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/5_workers/workerServer2.log > data/distributed/5_workers/time_txn_w3.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/5_workers/workerServer3.log > data/distributed/5_workers/time_txn_w4.csv
awk -F '\t' '{ if ($2!=0) print $1/$2}' data/distributed/5_workers/workerServer4.log > data/distributed/5_workers/time_txn_w5.csv
paste -d ',' data/distributed/1_worker/time_txn_w1.csv > data/distributed/1_worker/time_txn.csv
paste -d ',' data/distributed/2_workers/time_txn_w1.csv data/distributed/2_workers/time_txn_w2.csv > data/distributed/2_workers/time_txn.csv
paste -d ',' data/distributed/3_workers/time_txn_w1.csv data/distributed/3_workers/time_txn_w2.csv data/distributed/3_workers/time_txn_w3.csv > data/distributed/3_workers/time_txn.csv
paste -d ',' data/distributed/4_workers/time_txn_w1.csv data/distributed/4_workers/time_txn_w2.csv data/distributed/4_workers/time_txn_w3.csv data/distributed/4_workers/time_txn_w4.csv > data/distributed/4_workers/time_txn.csv
paste -d ',' data/distributed/5_workers/time_txn_w1.csv data/distributed/5_workers/time_txn_w2.csv data/distributed/5_workers/time_txn_w3.csv data/distributed/5_workers/time_txn_w4.csv data/distributed/5_workers/time_txn_w5.csv > data/distributed/5_workers/time_txn.csv
paste -d ',' data/distributed/1_worker/time_txn.csv data/distributed/2_workers/time_txn.csv data/distributed/3_workers/time_txn.csv data/distributed/4_workers/time_txn.csv data/distributed/5_workers/time_txn.csv
paste -d ',' data/distributed/1_worker/time_txn.csv data/distributed/2_workers/time_txn.csv data/distributed/3_workers/time_txn.csv data/distributed/4_workers/time_txn.csv data/distributed/5_workers/time_txn.csv > data/distributed/time_txn.csv
python plots/vPlot_big.py data/distributed/time_txn.csv plots/time_txn.pdf "W1.1,W2.1,W2.2,W3.1,W3.2,W3.3,W4.1,W4.2,W4.3,W4.4,W5.1,W5.2,W5.3,W5.4,W5.5" "Transactions Execution Time (microseconds)" "Worker Configurations"