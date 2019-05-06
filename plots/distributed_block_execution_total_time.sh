#awk -F '\t' '{print $5}' data/serial/serial.log > data/serial/be_ntt.csv
#awk -F '\t' '{print $6/1000}' data/serial/serial.log > data/serial/be_ctt.csv
awk -F '\t' '{print $1/1000}' data/serial/be_ctt.csv > data/serial/be_ctt_milli.csv


#awk -F '\t' '{print ($4 + $5)/1000}' data/1_worker/masterServer.log > data/1_worker/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/2_workers/masterServer.log > data/2_workers/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/3_workers/masterServer.log > data/3_workers/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/4_workers/masterServer.log > data/4_workers/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/5_workers/masterServer.log > data/5_workers/be_total_time.csv

awk -F ',' '{print $1}' data/distributed/1_worker/be_total_time.csv > data/distributed/1_worker/be_ntt.csv
awk -F ',' '{print $2/1000}' data/distributed/1_worker/be_total_time.csv > data/distributed/1_worker/be_ctt.csv
awk -F ',' '{print $1}' data/distributed/2_workers/be_total_time.csv > data/distributed/2_workers/be_ntt.csv
awk -F ',' '{print $2/1000}' data/distributed/2_workers/be_total_time.csv > data/distributed/2_workers/be_ctt.csv
awk -F ',' '{print $1}' data/distributed/3_workers/be_total_time.csv > data/distributed/3_workers/be_ntt.csv
awk -F ',' '{print $2/1000}' data/distributed/3_workers/be_total_time.csv > data/distributed/3_workers/be_ctt.csv
awk -F ',' '{print $1}' data/distributed/4_workers/be_total_time.csv > data/distributed/4_workers/be_ntt.csv
awk -F ',' '{print $2/1000}' data/distributed/4_workers/be_total_time.csv > data/distributed/4_workers/be_ctt.csv
awk -F ',' '{print $1}' data/distributed/5_workers/be_total_time.csv > data/distributed/5_workers/be_ntt.csv
awk -F ',' '{print $2/1000}' data/distributed/5_workers/be_total_time.csv > data/distributed/5_workers/be_ctt.csv


paste -d ',' data/serial/be_ntt.csv data/distributed/1_worker/be_ntt.csv data/distributed/2_workers/be_ntt.csv data/distributed/3_workers/be_ntt.csv data/distributed/4_workers/be_ntt.csv data/distributed/5_workers/be_ntt.csv > data/distributed/be_ntt.csv
python plots/vPlot_ntt.py data/distributed/be_ntt.csv plots/be_ntt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Normal Transaction Execution Time (microseconds)" "Configurations"

paste -d ',' data/serial/be_ctt_milli.csv data/distributed/1_worker/be_ctt.csv data/distributed/2_workers/be_ctt.csv data/distributed/3_workers/be_ctt.csv data/distributed/4_workers/be_ctt.csv data/distributed/5_workers/be_ctt.csv > data/distributed/be_ctt.csv
python plots/vPlot_ctt.py data/distributed/be_ctt.csv plots/be_ctt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Contract Transaction Execution Time (milliseconds)" "Configurations"