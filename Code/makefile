gensource: macro11 obj2ascii

macro11:
	~faustm/bin/macro11 ../TestFile/source.mac -o ../TestFile/source.obj -l ../TestFile/source.lst

obj2ascii: 
	~faustm/bin/obj2ascii ../TestFile/source.obj ../TestFile/source.ascii

all:	compile run 

compile: 
	gcc simulator.c simulator.h
run:
	./a.out

show: 
	vim ../file.txt 
