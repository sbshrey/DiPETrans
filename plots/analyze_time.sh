cat data/distributed/1_worker/log.out | grep MasterServer | grep analyze | awk -v OFS=',' '{print $5,$2}' > data/distributed/1_worker/analyze.csv
cat data/distributed/2_workers/log.out | grep MasterServer | grep analyze | awk -v OFS=',' '{print $5,$2}' > data/distributed/2_workers/analyze.csv
cat data/distributed/3_workers/log.out | grep MasterServer | grep analyze | awk -v OFS=',' '{print $5,$2}' > data/distributed/3_workers/analyze.csv
cat data/distributed/4_workers/log.out | grep MasterServer | grep analyze | awk -v OFS=',' '{print $5,$2}' > data/distributed/4_workers/analyze.csv
cat data/distributed/5_workers/log.out | grep MasterServer | grep analyze | awk -v OFS=',' '{print $5,$2}' > data/distributed/5_workers/analyze.csv
python plots/scraper.py
paste -d ',' data/distributed/1_worker/analyze_time.csv data/distributed/2_workers/analyze_time.csv data/distributed/3_workers/analyze_time.csv data/distributed/4_workers/analyze_time.csv data/distributed/5_workers/analyze_time.csv > data/distributed/analyze_time.csv
python plots/vPlot.py data/distributed/analyze_time.csv plots/analyze_time.pdf "2_workers,3_workers,4_workers,5_workers" "Analyze time (microseconds)" "Configurations"