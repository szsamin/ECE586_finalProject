CC = gcc
PROJ = ~shadman/ECE586_finalProject
ROOT =  /usr/bin
FAUST = ~faustm/bin

gensource: macro11 obj2ascii

macro11:
	@ ${FAUST}/macro11 ${PROJ}/TestFiles/source.mac -o ${PROJ}/TestFiles/source.obj -l ${PROJ}/Testfiles/source.lst	 

obj2ascii:
	@ ${FAUST}/obj2ascii ${PROJ}/TestFiles/source.obj ${PROJ}/TestFiles/source.ascii

all: compile run 

compile:
	echo $(CURDIR) 
	gcc -Wall simulator.h simulator.c 	 	 	
run:
	./a.out

show:
	vim ../file.txt 
