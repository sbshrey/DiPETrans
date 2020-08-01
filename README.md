# DiPETrans: A Framework for *Di*stributed *P*arallel *E*xecution of *Trans*actions of Blocks in Blockchain

*Shrey Baheti, Parwat Singh Anjana, Sathya Peri,and Yogesh Simmhan*

**ABSTRACT:** Contemporary blockchain such as Bitcoin and Ethereum execute transactions serially by miners and validators and determine the Proof-of-Work (PoW). Such serial execution is unable to exploit modern multi-core resources efficiently, hence limiting the system throughput and increasing the transaction acceptance latency.

The objective of this work is to increase the transaction throughput by introducing parallel transaction execution using a static analysis technique. We propose a framework DiPETransfor the distributed execution of the transactions in a block. Here, peers in the blockchain network form a community to execute the transactions and find the PoW parallelly, using a leader-follower approach. During mining, the leader statically analyzes the transactions, creates different groups (shards) of independent transactions, and distributes them to followers to execute them in parallel. After the transaction executes, the community’s compute power is utilized to solve the PoW concurrently. When a block is successfully created, the leader broadcasts the proposed block to other peers in the network for validation. On receiving a block, validators re-execute the block transactions and accept the block if they reach the same state as shared by the miner. Validation can also be done as a community, in parallel, following the same leader-follower approach as mining.

We report experiments using over 5 million real transactions from the Ethereum blockchain and execute them using our DiPETrans framework to empirically validate the benefits of our techniques over traditional sequential execution. We achieve a maximum speedup of 2.2× for the miner and 2.0× for the validator, with 100 to 500 transactions per block. Further, we achieve a peak of 5× end-to-end block creation speedup using a parallel miner over a serial miner when using 6 machines in the community.

## DiPETrans setup

The **DiPETrans** will work in any distributed 

### Part 1 : Setup distributed environment

User can containers or multiple nodes to create their own environment to run DiPETrans implementation. There is no limit on number of machines that user can create within community. The nodes part of the network must have passwordless ssh access. The current version has the hard coded ip and port information which will be modularised soon. In the later version, we will be adding a config file where users can mention the ip and port for the nodes and that will be used to communicate with the other nodes part of the network.

### Part 2 : Configuration 

1. Clone the repository. 
2. User need to update the masterServer.cpp, workerServer.cpp and masterClient.cpp to communicate with their nodes. If user wants to run within a single system then replace all ip values places with "localhost" else replaced it with your defined master and workers ips.
3. Make the changes to generate the executables.
4. Update run.py similar to changes made to above cpp files.
5. run the setup using the updated run.py script


## Technical Report
The technical report is available here: https://arxiv.org/abs/1906.11721
