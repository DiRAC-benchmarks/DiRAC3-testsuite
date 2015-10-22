These are the scripts to test the perfomance of /scratch file system on Archer.

Directory Structure:
____________________

IOR_DiRAC - POSIX tests
IOR_DiRAC_HDF5 - HDF5 tests
IOR_DiRAC_MPIIO - MPIIO tests


Level 2: Each of the IOR_DiRAC* directory contains
__________________________________________________

NS.FPS - Strong Scaling - File per process
NS.SHF - Strong Scaling - Shared file, across all the nodes

NW.FPS - Weak Scaling - File per process
NS.SHF - Weak Scaling - Shared file, across all the nodes


Level 3: Each of the N* directories contain:
___________________________________________


con.ior - IOR Configuration file

File Size: 256 GB
Transfer Size: 4mb
Varied Stripe sizes 


*.sub - Job submission scripts

Vary the number of nodes and file sizes and perform mpiruns

Contact:
______

sid.kashyap@ed.ac.uk
richard.rollins@ed.ac.uk
angus.lepper@ed.ac.uk


