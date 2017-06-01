Laurent LEQUIEVRE
laurent.lequievre@uca.fr

Juan Antonio Corrales Ramon
Juan-Antonio.Corrales-Ramon@sigma-clermont.fr

Youcef Mezouar
youcef.mezouar@sigma-clermont.fr

Institut Pascal
MACCS Team (http://ip.univ-bpclermont.fr/index.php/fr/maccs)
UMR6602 Clermont Ferrand

This git project contains a fork of FRI (Fast Research Interface library of Standford http://cs.stanford.edu/people/tkr/fri/html/) to be used with a specific port and IP address.
To use it, just add a new 'Network' entry in the 'init' file, like this :

[Network]
#Put your network IP here
#then, the interface will be choosed
ServerIP=192.168.100.253
ServerPortNumber=49938

Our platform needs these modifications because we have 2 kuka lwr4 + arms that have a port and a specific ip address.
See the files 49938-FRI-Driver.init and 49939-FRI-Driver.init in directory 'etc'


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

2bis- to compile on linux system, these directories must imperatively exist :
Linux/x86/release/obj
Linux/x86/release/bin
Linux/x86/release/lib

Linux/x86/debug/obj
Linux/x86/debug/bin
Linux/x86/debug/lib

Linux/x64/release/obj
Linux/x64/release/bin
Linux/x64/release/lib

Linux/x64/debug/obj
Linux/x64/debug/bin
Linux/x64/debug/lib

3- go to Linux directory and do :
make clean all


Remarks:
=======

To separate the 2 kuka arms (right and left) we have to set a specific ip address in the Dlrrc.ini file KRC computer side.
This file is in the c:\krc\ROBOTER\INIT directory.
Once this file updated we need to reboot the KRC computer.

For the right arm, we set the IP address to 192.168.100.120
For the left one, we set the IP address to 192.168.100.102

Of course, we have to take care of that in our script net file (In the ROS workspace -> ~/projects/ros_sigma_platform_fri_ws/src/script_net/net_kuka_sigma.sh)

