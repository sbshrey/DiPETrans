# DiPETrans: A Framework for *Di*stributed *P*arallel *E*xecution of *Trans*actions of Blocks in Blockchain

*Shrey Baheti, Parwat Singh Anjana, Sathya Peri,and Yogesh Simmhan*

**ABSTRACT:** In most of the modern day blockchain, transactions are executed serially by both miners and validators; also, PoW is determined serially. The serial execution limits the system throughput and increases transaction acceptance latency, even unable to exploit the modern multi-core resources efficiently.

In this work, we try to increase the throughput by introducing parallel execution of the transactions using a static analysis based technique. We propose a framework **DiPETrans** for the distributed parallel execution of block transactions. In \emph{DiPETrans}, trusted peers in the blockchain network form a community to execute the transactions and to find the PoW parallelly. The community follows a master-slave approach for parallel execution. The core idea is that master analyzes the transactions using a static analysis based technique, creates different groups (shards) of non-conflicting transactions, and distribute shards to workers (community members) to execute them parallelly. After transaction execution, communities compute power is utilized to find PoW parallelly. On successful creation of a block, the master broadcasts the proposed block to other peers in the network for validation. On receiving a block, validators re-executes the block transactions, either parallelly (community) or serially (solo validators). If they reach the same state as shared by the miner, then accept the block otherwise reject. We proposed two different approaches for the validator, one in which miner share the shard (dependency) information in the block to help validators to execute the transaction deterministically parallel, while in another approach no dependency information is shared with validators and validator needs to determine the dependencies using static analysis of the block transactions.

We performed experiments with historical data from Ethereum blockchain and achieved linear speedup for transaction execution and end-to-end block creation with up to 5 workers in the community. We experimented by varying the number of transactions per block from 100 to 500 and obtained maximum speedup of 2.18x for *miner*, 2.04x for *info sharing validator*, and 2.02x for *no info sharing validator*. **DiPETrans** is first of its kind, and we will be evolving it to provide better performance. 


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

