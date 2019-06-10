import sys
import os

dir_path = "data/block16/"



#size = {'1':4793,'2':7325,'4':12240,'8':23371,'16':43998}

for i in range(1,6):
	print i
	normal_txn_time = {}
	contract_txn_time = {}

	for j in range(0,100):
		normal_txn_time[j] = 0
		contract_txn_time[j] = 0

	path = ""

	if i==1:
		path = "{}{}_worker/".format(dir_path,i)
	else:
		path = "{}{}_workers/".format(dir_path,i)
		
	for j in range(1,i+1):
		filename = path + "be_ntt_{}.log".format(j)
		os.system('wc -l {}'.format(filename))
		print "filename", filename
		f = open(filename,'r')
		data = f.readlines()
		f.close()
		for k in range(len(data)):
			ntt = float(data[k])
			if float(normal_txn_time[k]) < ntt:
				normal_txn_time[k] = ntt

		filename = path + "be_ctt_{}.log".format(j)
		print filename
		f = open(filename,'r')
		data = f.readlines()
		f.close()
		for k in range(len(data)):
			#print data[k]
			ctt = float(data[k])/1000
			if float(contract_txn_time[k]) < ctt:
				contract_txn_time[k] = ctt


	with open(path + "be_ntt.csv",'w') as file:
		for k in range(len(normal_txn_time)):
			if normal_txn_time[k] > 0:
				file.write(str(normal_txn_time[k])+"\n")

	with open(path + "be_ctt.csv",'w') as file:
		for k in range(len(contract_txn_time)):
			if contract_txn_time[k] > 0:
				file.write(str(contract_txn_time[k])+"\n")


		#print "awk {0}{1}be_e2e.csv > {0}{1}be_e2e_milli.csv".format(dir_path,path)
		#os.system("awk '{print $1/1000}' {0}{1}be_e2e.csv > {0}{1}be_e2e_milli.csv".format(dir_path,path))
	
	#print "awk  {0}serial/be_ctt.csv > {0}serial/be_ctt_milli.csv".format(dir_path)
	#print "awk  {0}serial/be_e2e.csv > {0}serial/be_e2e_milli.csv".format(dir_path)
	#os.system("awk '{print $1/1000\}' {}serial/be_ctt.csv > {}serial/be_ctt_milli.csv")
	#os.system("awk '\{print $1/1000\}' {}serial/be_e2e.csv > {}serial/be_e2e_milli.csv")


os.system("paste -d ',' {0}serial/be_ntt.csv {0}1_worker/be_ntt.csv {0}2_workers/be_ntt.csv {0}3_workers/be_ntt.csv {0}4_workers/be_ntt.csv {0}5_workers/be_ntt.csv > {0}be_ntt.csv".format(dir_path))
os.system('python plots/vPlot_ntt.py {0}be_ntt.csv {0}be_ntt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Normal Transaction Execution Time (microseconds)" "Configurations"'.format(dir_path))

os.system("paste -d ',' {0}serial/be_ctt_milli.csv {0}1_worker/be_ctt.csv {0}2_workers/be_ctt.csv {0}3_workers/be_ctt.csv {0}4_workers/be_ctt.csv {0}5_workers/be_ctt.csv > {0}be_ctt.csv".format(dir_path))
os.system('python plots/vPlot_ctt.py {0}be_ctt.csv {0}be_ctt.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "Contract Transaction Execution Time (milliseconds)" "Configurations"'.format(dir_path))

os.system("paste -d ',' {0}serial/be_e2e_milli.csv {0}1_worker/be_e2e_milli.csv {0}2_workers/be_e2e_milli.csv {0}3_workers/be_e2e_milli.csv {0}4_workers/be_e2e_milli.csv {0}5_workers/be_e2e_milli.csv > {0}be_e2e_milli.csv".format(dir_path))
os.system('python plots/vPlot_e2e.py {0}be_e2e_milli.csv {0}be_e2e_milli.pdf "serial,1_worker,2_workers,3_workers,4_workers,5_workers" "End-to-End Execution Time (milliseconds)" "Configurations"'.format(dir_path))
	
