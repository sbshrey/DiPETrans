awk -F '\t' '{print $2/$1*1000}' data/distributed/1_worker/workerServer0.log > data/distributed/1_worker/txn_time_w1.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/2_workers/workerServer0.log > data/distributed/2_workers/txn_time_w1.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/2_workers/workerServer1.log > data/distributed/2_workers/txn_time_w2.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/3_workers/workerServer0.log > data/distributed/3_workers/txn_time_w1.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/3_workers/workerServer1.log > data/distributed/3_workers/txn_time_w2.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/3_workers/workerServer2.log > data/distributed/3_workers/txn_time_w3.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/4_workers/workerServer0.log > data/distributed/4_workers/txn_time_w1.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/4_workers/workerServer1.log > data/distributed/4_workers/txn_time_w2.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/4_workers/workerServer2.log > data/distributed/4_workers/txn_time_w3.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/4_workers/workerServer3.log > data/distributed/4_workers/txn_time_w4.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/5_workers/workerServer0.log > data/distributed/5_workers/txn_time_w1.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/5_workers/workerServer1.log > data/distributed/5_workers/txn_time_w2.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/5_workers/workerServer2.log > data/distributed/5_workers/txn_time_w3.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/5_workers/workerServer3.log > data/distributed/5_workers/txn_time_w4.csv
awk -F '\t' '{print $2/$1*1000}' data/distributed/5_workers/workerServer4.log > data/distributed/5_workers/txn_time_w5.csv
paste -d ',' data/distributed/1_worker/txn_time_w1.csv > data/distributed/1_worker/txn_time.csv
cat data/distributed/1_worker/txn_time_w1.csv >> data/distributed/1_worker/agg_txn_time.csv
paste -d ',' data/distributed/2_workers/txn_time_w1.csv data/distributed/2_workers/txn_time_w2.csv > data/distributed/2_workers/txn_time.csv
cat data/distributed/2_workers/txn_time_w1.csv >> data/distributed/2_workers/agg_txn_time.csv
cat data/distributed/2_workers/txn_time_w2.csv >> data/distributed/2_workers/agg_txn_time.csv
paste -d ',' data/distributed/3_workers/txn_time_w1.csv data/distributed/3_workers/txn_time_w2.csv data/distributed/3_workers/txn_time_w3.csv > data/distributed/3_workers/txn_time.csv
cat data/distributed/3_workers/txn_time_w1.csv >> data/distributed/3_workers/agg_txn_time.csv
cat data/distributed/3_workers/txn_time_w2.csv >> data/distributed/3_workers/agg_txn_time.csv
cat data/distributed/3_workers/txn_time_w3.csv >> data/distributed/3_workers/agg_txn_time.csv
paste -d ',' data/distributed/4_workers/txn_time_w1.csv data/distributed/4_workers/txn_time_w2.csv data/distributed/4_workers/txn_time_w3.csv data/distributed/4_workers/txn_time_w4.csv > data/distributed/4_workers/txn_time.csv
cat data/distributed/4_workers/txn_time_w1.csv >> data/distributed/4_workers/agg_txn_time.csv
cat data/distributed/4_workers/txn_time_w2.csv >> data/distributed/4_workers/agg_txn_time.csv
cat data/distributed/4_workers/txn_time_w3.csv >> data/distributed/4_workers/agg_txn_time.csv
cat data/distributed/4_workers/txn_time_w4.csv >> data/distributed/4_workers/agg_txn_time.csv
paste -d ',' data/distributed/5_workers/txn_time_w1.csv data/distributed/5_workers/txn_time_w2.csv data/distributed/5_workers/txn_time_w3.csv data/distributed/5_workers/txn_time_w4.csv data/distributed/5_workers/txn_time_w5.csv > data/distributed/5_workers/txn_time.csv
cat data/distributed/5_workers/txn_time_w1.csv >> data/distributed/5_workers/agg_txn_time.csv
cat data/distributed/5_workers/txn_time_w2.csv >> data/distributed/5_workers/agg_txn_time.csv
cat data/distributed/5_workers/txn_time_w3.csv >> data/distributed/5_workers/agg_txn_time.csv
cat data/distributed/5_workers/txn_time_w4.csv >> data/distributed/5_workers/agg_txn_time.csv
cat data/distributed/5_workers/txn_time_w5.csv >> data/distributed/5_workers/agg_txn_time.csv
paste -d ',' data/distributed/1_worker/txn_time.csv data/distributed/2_workers/txn_time.csv data/distributed/3_workers/txn_time.csv data/distributed/4_workers/txn_time.csv data/distributed/5_workers/txn_time.csv > data/distributed/txn_time.csv
paste -d ',' data/distributed/1_worker/agg_txn_time.csv data/distributed/2_workers/agg_txn_time.csv data/distributed/3_workers/agg_txn_time.csv data/distributed/4_workers/agg_txn_time.csv data/distributed/5_workers/agg_txn_time.csv > data/distributed/agg_txn_time.csv
python plots/vPlot_big.py data/distributed/txn_time.csv plots/txn_time.pdf "W1.1,W2.1,W2.2,W3.1,W3.2,W3.3,W4.1,W4.2,W4.3,W4.4,W5.1,W5.2,W5.3,W5.4,W5.5" "#Transactions/milliseconds" "Worker Configurations"
python plots/vPlot.py data/distributed/agg_txn_time.csv plots/agg_txn_time.pdf "1_worker,2_workers,3_workers,4_workers,5_workers" "Aggregated #Transactions/milliseconds" "Worker Configurations"