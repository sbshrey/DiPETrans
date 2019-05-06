#rm -rf data/bigquery/ethereum_data_1_1/1_worker/be_ctt.csv data/1_worker/be_ntt.csv
#rm -rf data/2_workers/be_ntt.csv data/2_workers/be_ctt.csv
#rm -rf data/3_workers/be_ntt.csv data/3_workers/be_ctt.csv
#rm -rf data/4_workers/be_ntt.csv data/4_workers/be_ctt.csv
#rm -rf data/5_workers/be_ntt.csv data/5_workers/be_ctt.csv

#awk -F '\t' '{print $5}' data/serial/serial.log > data/serial/be_ntt.csv
#awk -F '\t' '{print $6/1000}' data/serial/serial.log > data/serial/be_ctt.csv
#awk -F '\t' '{print $1/1000}' data/serial/be_ctt.csv > data/serial/be_ctt_milli.csv


#awk -F '\t' '{print ($4 + $5)/1000}' data/1_worker/masterServer.log > data/1_worker/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/2_workers/masterServer.log > data/2_workers/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/3_workers/masterServer.log > data/3_workers/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/4_workers/masterServer.log > data/4_workers/be_total_time.csv
#awk -F '\t' '{print ($4 + $5)/1000}' data/5_workers/masterServer.log > data/5_workers/be_total_time.csv


#cat data/1_worker/be_ctt_1.log  > data/1_worker/be_ctt.csv
#cat data/2_workers/be_ctt_1.log data/2_workers/be_ctt_2.log > data/2_workers/be_ctt.csv
#cat data/3_workers/be_ctt_1.log data/3_workers/be_ctt_2.log data/3_workers/be_ctt_3.log > data/3_workers/be_ctt.csv
#cat data/4_workers/be_ctt_1.log data/4_workers/be_ctt_2.log data/4_workers/be_ctt_3.log data/4_workers/be_ctt_4.log > data/4_workers/be_ctt.csv
#cat data/5_workers/be_ctt_1.log data/5_workers/be_ctt_2.log data/5_workers/be_ctt_3.log data/5_workers/be_ctt_4.log data/5_workers/be_ctt_5.log > data/5_workers/be_ctt.csv


#cat data/1_worker/be_ntt_1.log  > data/1_worker/be_ntt.csv
#cat data/2_workers/be_ntt_1.log data/2_workers/be_ntt_2.log > data/2_workers/be_ntt.csv
#cat data/3_workers/be_ntt_1.log data/3_workers/be_ntt_2.log data/3_workers/be_ntt_3.log > data/3_workers/be_ntt.csv
#cat data/4_workers/be_ntt_1.log data/4_workers/be_ntt_2.log data/4_workers/be_ntt_3.log data/4_workers/be_ntt_4.log > data/4_workers/be_ntt.csv
#cat data/5_workers/be_ntt_1.log data/5_workers/be_ntt_2.log data/5_workers/be_ntt_3.log data/5_workers/be_ntt_4.log data/5_workers/be_ntt_5.log > data/5_workers/be_ntt.csv



#awk -F ',' '{print $1}' data/1_worker/be_total_time.csv > data/1_worker/be_ntt.csv
#awk -F ',' '{print $1/1000}' data//1_worker/be_total_time.csv > data/1_worker/be_ctt.csv
#awk -F ',' '{print $1}' data/2_workers/be_total_time.csv > data/2_workers/be_ntt.csv
#awk -F ',' '{print $1/1000}' data/2_workers/be_total_time.csv > data/2_workers/be_ctt.csv
#awk -F ',' '{print $1}' data/3_workers/be_total_time.csv > data/3_workers/be_ntt.csv
#awk -F ',' '{print $1/1000}' data/3_workers/be_total_time.csv > data/3_workers/be_ctt.csv
#awk -F ',' '{print $1}' data/4_workers/be_total_time.csv > data/4_workers/be_ntt.csv
#awk -F ',' '{print $1/1000}' data/4_workers/be_total_time.csv > data/4_workers/be_ctt.csv
#awk -F ',' '{print $1}' data/5_workers/be_total_time.csv > data/5_workers/be_ntt.csv
#awk -F ',' '{print $1/1000}' data/5_workers/be_total_time.csv > data/5_workers/be_ctt.csv

#awk -F '\t' '{print $1/1000}' data/serial/be_ntt.csv > data/serial/be_ctt_milli.csv
#awk -F '\t' '{print $1/1000}' data/serial/be_ctt.csv > data/serial/be_ctt_milli.csv


#paste -d ',' data/serial/be_ntt.csv data/1_worker/be_ntt.csv data/2_workers/be_ntt.csv data/3_workers/be_ntt.csv data/4_workers/be_ntt.csv data/5_workers/be_ntt.csv > data/be_ntt.csv
#python plots/vPlot_ntt.py data/be_ntt.csv plots/be_ntt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Normal Transaction Execution Time (microseconds)" "Configurations"
#
#paste -d ',' data/serial/be_ctt.csv data/1_worker/be_ctt.csv data/2_workers/be_ctt.csv data/3_workers/be_ctt.csv data/4_workers/be_ctt.csv data/5_workers/be_ctt.csv > data/be_ctt.csv
#python plots/vPlot_ctt.py data/be_ctt.csv plots/be_ctt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Contract Transaction Execution Time (milliseconds)" "Configurations"


#paste -d ',' data/bigquery/ethereum_data_1_1/serial/be_ntt.csv data/bigquery/ethereum_data_1_1/1_worker/be_ntt.csv data/bigquery/ethereum_data_1_1/2_workers/be_ntt.csv data/bigquery/ethereum_data_1_1/3_workers/be_ntt.csv data/bigquery/ethereum_data_1_1/4_workers/be_ntt.csv data/bigquery/ethereum_data_1_1/5_workers/be_ntt.csv > data/bigquery/ethereum_data_1_1/be_ntt.csv
#python plots/vPlot_ntt.py data/bigquery/ethereum_data_1_1/be_ntt.csv plots/bigquery/ethereum_data_1_1/be_ntt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Normal Transaction Execution Time (microseconds)" "Configurations"

#paste -d ',' data/bigquery/ethereum_data_1_1/serial/be_ctt.csv data/bigquery/ethereum_data_1_1/1_worker/be_ctt.csv data/bigquery/ethereum_data_1_1/2_workers/be_ctt.csv data/bigquery/ethereum_data_1_1/3_workers/be_ctt.csv data/bigquery/ethereum_data_1_1/4_workers/be_ctt.csv data/bigquery/ethereum_data_1_1/5_workers/be_ctt.csv > data/bigquery/ethereum_data_1_1/be_ctt.csv
#python plots/vPlot_ctt.py data/bigquery/ethereum_data_1_1/be_ctt.csv plots/bigquery/ethereum_data_1_1/be_ctt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Contract Transaction Execution Time (microseconds)" "Configurations"




awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/1_worker/be_e2e.csv > data/bigquery/ethereum_data_1_1/1_worker/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/2_workers/be_e2e.csv > data/bigquery/ethereum_data_1_1/2_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/3_workers/be_e2e.csv > data/bigquery/ethereum_data_1_1/3_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/4_workers/be_e2e.csv > data/bigquery/ethereum_data_1_1/4_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/5_workers/be_e2e.csv > data/bigquery/ethereum_data_1_1/5_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/serial/be_ctt.csv > data/bigquery/ethereum_data_1_1/serial/be_ctt_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_1/serial/be_e2e.csv > data/bigquery/ethereum_data_1_1/serial/be_e2e_milli.csv

awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/1_worker/be_e2e.csv > data/bigquery/ethereum_data_1_2/1_worker/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/2_workers/be_e2e.csv > data/bigquery/ethereum_data_1_2/2_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/3_workers/be_e2e.csv > data/bigquery/ethereum_data_1_2/3_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/4_workers/be_e2e.csv > data/bigquery/ethereum_data_1_2/4_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/5_workers/be_e2e.csv > data/bigquery/ethereum_data_1_2/5_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/serial/be_ctt.csv > data/bigquery/ethereum_data_1_2/serial/be_ctt_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_2/serial/be_e2e.csv > data/bigquery/ethereum_data_1_2/serial/be_e2e_milli.csv

awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/1_worker/be_e2e.csv > data/bigquery/ethereum_data_1_4/1_worker/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/2_workers/be_e2e.csv > data/bigquery/ethereum_data_1_4/2_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/3_workers/be_e2e.csv > data/bigquery/ethereum_data_1_4/3_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/4_workers/be_e2e.csv > data/bigquery/ethereum_data_1_4/4_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/5_workers/be_e2e.csv > data/bigquery/ethereum_data_1_4/5_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/serial/be_ctt.csv > data/bigquery/ethereum_data_1_4/serial/be_ctt_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_4/serial/be_e2e.csv > data/bigquery/ethereum_data_1_4/serial/be_e2e_milli.csv

awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/1_worker/be_e2e.csv > data/bigquery/ethereum_data_1_8/1_worker/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/2_workers/be_e2e.csv > data/bigquery/ethereum_data_1_8/2_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/3_workers/be_e2e.csv > data/bigquery/ethereum_data_1_8/3_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/4_workers/be_e2e.csv > data/bigquery/ethereum_data_1_8/4_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/5_workers/be_e2e.csv > data/bigquery/ethereum_data_1_8/5_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/serial/be_ctt.csv > data/bigquery/ethereum_data_1_8/serial/be_ctt_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_8/serial/be_e2e.csv > data/bigquery/ethereum_data_1_8/serial/be_e2e_milli.csv

awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/1_worker/be_e2e.csv > data/bigquery/ethereum_data_1_16/1_worker/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/2_workers/be_e2e.csv > data/bigquery/ethereum_data_1_16/2_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/3_workers/be_e2e.csv > data/bigquery/ethereum_data_1_16/3_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/4_workers/be_e2e.csv > data/bigquery/ethereum_data_1_16/4_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/5_workers/be_e2e.csv > data/bigquery/ethereum_data_1_16/5_workers/be_e2e_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/serial/be_ctt.csv > data/bigquery/ethereum_data_1_16/serial/be_ctt_milli.csv
awk -F '\t' '{print $1/1000}' data/bigquery/ethereum_data_1_16/serial/be_e2e.csv > data/bigquery/ethereum_data_1_16/serial/be_e2e_milli.csv