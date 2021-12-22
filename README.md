# DiPETrans: A Framework for *Di*stributed *P*arallel *E*xecution of *Trans*actions of Blocks in Blockchain

*Shrey Baheti, Parwat Singh Anjana, Sathya Peri,and Yogesh Simmhan*

## Accepted at: 
Concurrency and Computation: Practice and Experience - Wiley, DOI: 10.1002/CPE.6804

## Technical Report
The technical report is available here: https://arxiv.org/abs/1906.11721

## DiPETrans setup

### Part 1 : Setup distributed environment

User can containers or multiple nodes to create their own environment to run DiPETrans implementation. There is no limit on number of machines that user can create within community. The nodes part of the network must have passwordless ssh access. The current version has the hard coded ip and port information which will be modularised soon. In the later version, we will be adding a config file where users can mention the ip and port for the nodes and that will be used to communicate with the other nodes part of the network.

### Part 2 : Configuration 

1. Clone the repository. 
2. User need to update the masterServer.cpp, workerServer.cpp and masterClient.cpp to communicate with their nodes. If user wants to run within a single system then replace all ip values places with "localhost" else replaced it with your defined master and workers ips.
3. Make the changes to generate the executables.
4. Update run.py similar to changes made to above cpp files.
5. run the setup using the updated run.py script.
