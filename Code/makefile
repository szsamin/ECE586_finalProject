CC = /usr/bin
PATH = ~shadman/ECE586_finalProject
ROOT =  /usr/bin
FAUST = ~faustm/bin

gensource: macro11 obj2ascii

macro11:
	@ ${FAUST}/macro11 ${PATH}/TestFiles/source.mac -o ${PATH}/TestFiles/source.obj -l ${PATH}/Testfiles/source.lst	 

obj2ascii:
	@ ${FAUST}/obj2ascii ${PATH}/TestFiles/source.obj ${PATH}/TestFiles/source.ascii

all: compile run 

compile: 
	${CC} -o simulator simulator.c

simulator:
	${CC} -o simulator simulator.c

header: simulator
	${CC} -c simulator.h -o header.o
	
output: header
	${CC} code.o header.o -o output	

	 	 	
run:
	./a.out

show:
	vim ../file.txt 
