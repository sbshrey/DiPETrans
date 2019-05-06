awk -F '\t' '{print $4}' data/serial/serial.log > data/serial/total_time.csv
awk -F '\t' '{print $4}' data/distributed/1_worker/masterClient.log > data/distributed/1_worker/total_time.csv
awk -F '\t' '{print $4}' data/distributed/2_workers/masterClient.log > data/distributed/2_workers/total_time.csv
awk -F '\t' '{print $4}' data/distributed/3_workers/masterClient.log > data/distributed/3_workers/total_time.csv
awk -F '\t' '{print $4}' data/distributed/4_workers/masterClient.log > data/distributed/4_workers/total_time.csv
awk -F '\t' '{print $4}' data/distributed/5_workers/masterClient.log > data/distributed/5_workers/total_time.csv
paste -d ',' data/serial/total_time.csv data/distributed/1_worker/total_time.csv data/distributed/2_workers/total_time.csv data/distributed/3_workers/total_time.csv data/distributed/4_workers/total_time.csv data/distributed/5_workers/total_time.csv > data/total_time.csv
python plots/vPlot.py data/total_time.csv plots/total_time.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "End-to-End Time (microseconds)" "Configurations"