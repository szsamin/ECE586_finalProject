Theory of Operation: 
 
The makefile provides recipes to allow the user to run different type commands. Here are a list of the recipes provided inside the makefile.
 
all - run a .mac files through the simulator  
All - run a .ascii file through the simulator
macro11 - converts mac file to an object file
obj2ascii  - converts object file to an ascii file 
compile  - compiles the header and C files 
run  - Runs the simulation environment
show - displays the branch and trace files side by side
trace - Just displays the trace file
branch - Just displays the branch trace file
clean - cleans temporary files before generating new files
 
Flags used :
DEBUG - allow to print debug statements
STEP - allows the user to do a step by step debugging
A - enables memory alignment error
PRINTREG - Prints Register and Flag contents for each Instruction
 
Variables used:
TEST -  Input the filename either mac or ascii. The files should be put inside the folder Testfiles 
CFLAGS -  Input the flags needed. 
 
Execution Steps:
 
// Change the working directory to code
$ cd Code


// If the user just wants to run all the files without debugging and load a mac file 
$ make all TEST=filename.mac CFLAGS="-D A"

	OR 

// If the user just wants to run all the file with debugging and load a mac file$ make all TEST=filename.mac CFLAGS="-D A -D STEP -D DEBUG -D PRINTREG"
	
OR 
 
// If the user just wants to run all the file with debugging and load an ascii  file 
$ make ALL TEST=filename.ascii CFLAGS="-D A -D STEP -D DEBUG -D PRINTREG"
 
// Display trace file side by side
$ make show 
 
 
