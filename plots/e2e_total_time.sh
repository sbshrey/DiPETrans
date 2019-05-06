awk -F '\t' '{print $7/1000000}' data/serial/serial.log > data/serial/total_time.csv
awk -F '\t' '{print $7/1000000}' data/1_worker/masterServer.log > data/1_worker/total_time.csv
awk -F '\t' '{print $7/1000000}' data/2_workers/masterServer.log > data/2_workers/total_time.csv
awk -F '\t' '{print $7/1000000}' data/3_workers/masterServer.log > data/3_workers/total_time.csv
awk -F '\t' '{print $7/1000000}' data/4_workers/masterServer.log > data/4_workers/total_time.csv
awk -F '\t' '{print $7/1000000}' data/5_workers/masterServer.log > data/5_workers/total_time.csv
paste -d ',' data/serial/total_time.csv data/1_worker/total_time.csv data/2_workers/total_time.csv data/3_workers/total_time.csv data/4_workers/total_time.csv data/5_workers/total_time.csv > data/e2e_total_time.csv
python plots/vPlot.py data/e2e_total_time.csv plots/e2e_total_time.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "End-to-End Time (seconds)" "Configurations"