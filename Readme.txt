Laurent LEQUIEVRE
Institut Pascal
UMR6602 Clermont Ferrand

1- to compile with ubuntu 14.04 we need to install :
sudo apt-get install g++-multilib

2- to compile with 32 bits and 64 bits we have to uncomment in the file ExternalTargets.global :
from line 68 :
# ################################################## Uncommment from here
	make -k all_debug_x64
	@echo $(LINE2)
	@echo $(LINE2)
	@echo $(LINE2)
	@echo $(LINE2)
	make -k all_release_x64
# ################################################## to here to build for X64.

and from line 124 :

################################################### Uncommment from here
	make -k clean_debug_x64
	@echo $(LINE2)
	@echo $(LINE2)
	@echo $(LINE2)
	@echo $(LINE2)		
	make -k clean_release_x64
################################################### to here to build for X64.

3- go to Linux directory and do :
make clean all

Becareful -> the directories x64 and x86 inside the Linux directory should exist !

Remarks:
=======

To separate the 2 kuka arms (right and left) we have to set a specific ip address in the Dlrrc.ini file KRC computer side.
This file is in the c:\krc\ROBOTER\INIT directory.
Once this file updated we need to reboot the KRC computer.

For the right arm, we set the IP address to 192.168.100.120
For the left one, we set the IP address to 192.168.100.102

Of course, we have to take care of that in our script net file (In the ROS workspace -> ~/projects/ros_sigma_platform_fri_ws/src/script_net/net_kuka_sigma.sh)

