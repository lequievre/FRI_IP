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
