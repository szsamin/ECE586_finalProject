PATH = ~shadman/ECE586_finalProject

gensource: macro11 obj2ascii

macro11:
	~faustm/bin/macro11 ${PATH}/TestFiles/source.mac -o ${PATH}/TestFiles/source.obj -l ${PATH}/Testfiles/source.lst	 

obj2ascii: 
	~faustm/bin/obj2ascii ${PATH}/TestFiles/source.obj ${PATH}/TestFiles/txt.ascii

all:	compile run 

compile: 
	gcc simulator.c simulator.h
run:
	./a.out

show: 
	vim ../file.txt 
