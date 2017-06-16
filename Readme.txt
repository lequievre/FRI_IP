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

Of course, we have to take care of that in our net script file 'net_kuka_sigma.sh' (In 'script_net' directory).

Our tests files are in 'src/IP_Tests' directory.

About Xenomai 3:
================
It is not possible to compile a "shared" library with the LD_FLAGS given by "xeno-config --posix --ldflags" tool.
We need to use "xeno-config --posix --no-auto-init --ldflags"

"--no-auto-init" option :
-------------------------
All Xenomai APIs but POSIX are based on the Copperplate library, which automatically initializes when the application process starts. --no-auto-init can be passed to disable such initialization. In such a case, the application code shall call the copperplate_init() routine manually, as part of its initialization chores on behalf on the main() routine, before any real-time service is invoked.

This flag makes sense when passed along with --ldflags only. xeno-config enables the Copperplate auto-init feature by default.

cf :
https://xenomai.org/pipermail/xenomai/2016-September/036748.html
https://www.mail-archive.com/search?l=xenomai@xenomai.org&q=subject:%22%5C%5BXenomai%5C%5D+Cannot+create+a+share+library+linked+against+Xenomai+libs%22&o=newest&f=1

To resolve LD_LIBRARY_PATH with xenomai libraries:
--------------------------------------------------

1- Add a conf file in the directory /etc/ld.so.conf.d
for example xenomai_3_0_2.conf that contains :
# xenomai 3.0.2 conf
/usr/xenomai_3_0_2/lib

2- After that, launch the command : sudo ldconfig



 


