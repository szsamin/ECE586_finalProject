TEST ?= oopTest.mac
CFLAGS ?= 

CC = gcc
PROJ = ~shadman/ECE586_finalProject
ROOT =  /usr/bin
FAUST = ~faustm/bin


all:	gensource compile run

ALL:	movascii compile run 

gensource: macro11 obj2ascii

movascii:
	@ cp ${PROJ}/TestFiles/$(TEST) ${PROJ}/TestFiles/source.ascii 

macro11:
	@ ${FAUST}/macro11 ${PROJ}/TestFiles/${TEST} -o ${PROJ}/TestFiles/source.obj -l ${PROJ}/Testfiles/source.lst	 

obj2ascii:
	@ ${FAUST}/obj2ascii ${PROJ}/TestFiles/source.obj ${PROJ}/TestFiles/source.ascii

compile:	 
	@ ${CC} ${CFLAGS} -Wall simulator.h simulator.c `pkg-config --libs --cflags gtk+-2.0` 	 	 	
run:
	@ ./a.out

show: 
	@ pr -m -t tracefile.txt branchtrace.txt

trace:	
	@ cat tracefile.txt

branch:
	@ cat branchtrace.txt

clean: 
	@ rm ${PROJ}/TestFiles/source.obj ${PROJ}/TestFiles/source.ascii ${PROJ}/Code/branchtrace.txt ${PROJ}/Code/tracefile.txt 
