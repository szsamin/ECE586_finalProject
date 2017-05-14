/* Simulator C file 
   Author: Apurva Gandole, Chandan Muralidhar, Shadmna Samin, Sourabh Balakrishna
  
   Description: 

*/ 

// Library Declarations 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include "../Code/simulator.h" 


// Global data type initializations
char dataStr[MEM];  				
unsigned char mem[MEM]; 		// Memory size declaration 
unsigned short int Reg [8]; 		// Register size declaration 
unsigned short int PSW; 		// 

int main(int argc,int argv){
          /* initial step read file */ 
	  char *Source = "../TestFiles/source.ascii";   
	  open_file(Source); 
	  
	  int done = 0;

	  /* Initial Starting address */ 
          Reg[PC] = (argc == 2) ? argv : 0; 
	  /* Add a command line if else - if the user does not specify a starting address, the starting address defaults to 0 */ 
	  
	  unsigned short fetched_instruction; 
          long int instruction_counter = 0; 

	  /* Trace file generatin Initialization */ 
	  FILE * trace = fopen("tracefile.txt","w"); 
	  if(trace == NULL){
		printf("Error opening trace file!\n"); 
	        exit(1); 
	  } 

 	  
	  done = 1; // Ignores the Fetch > Decode > Execute for now 
	  while(!done){ 
	   	// Fetch Instruction  
	  	fetched_instruction = read_mem(trace,INSTRUCTION_FETCH,Reg[PC]); 
	  	
		// Decode Instruction  
		


		// Execute Instruction 
		instruction_counter++; 
	 
	
		// Increment 
		
	 }
	  	
	

	  

	  /* Throw data into mem */ 
			
	  /* Display function */ 
	  display(); 

	  /* Trace file end */ 
	  fclose(trace); 
	  return(0); 
}

void display(){ 
	/* Generate an output text file */ 
	FILE *f = fopen("../file.txt","w");
	if(f == NULL){
		printf("Error opening file!\n"); 
		exit(1); 
	}
	    	 
	/* Print everything in the memory */ 
	for(int i = 0; i < sizeof(mem); i++){
	    fprintf(f,"-----------------------------------------\n"); 
	    fprintf(f,"|   Content - %0o  |     Address - %0x	| \n",mem[i],&mem[i]);
	    fprintf(f,"-----------------------------------------\n"); 
	}
	
	fclose(f); 
	return;  
}

/* Read Memory Function */
/* Return value from read_mem - either instruction or memory content will be 16 bit return value */  
unsigned short read_mem(FILE *trace,unsigned short type, unsigned short address){
		
		unsigned short mem_value; 

		/* Check for odd memory address request - Address allignment check */
		if((address & 0x0001)){
			printf("Unalligned memory address - Read memory request error! Address = %o",address); 
			exit(1); 
		}

		/* Read mem */ 
		if(type == DATA_READ){
			mem_value = mem[address];

		} 
		/* Read instruction set */ 
		else if(type == INSTRUCTION_FETCH){
			mem_value = mem[address];
		  
		} 
		/* Trace file generation */
		fprintf(trace,"%d %o\n",type,address); 
		return mem_value; 
}


/* Write to Memory function */ 
void write_mem(FILE *trace,unsigned short type, unsigned short address, unsigned short data){
		if((address & 0x0001)){
			printf("Unalligned memory address - Write memory request error! Address = %o", address);
			exit(1); 
		}		
		
		mem[address] = data;
		/* Trace file generation */ 
		fprintf(trace,"%d %o\n",type,address); 
		return; 						
}



int open_file(char *arr){ 
	/* Opens the filename pointed to - 'r' opens a file for reading */ 
 	FILE * fp = fopen(arr,"r"); 
	char *character;
        int scan_file;
	unsigned int octal_value;
        unsigned short int mem_pointer = 0;
        Reg[PC] = 0; 	
	
	/* Check if the file exits */ 
	if(fp == NULL){
		perror("Error in opening file"); 
		return(-1); 
	}

	
	while(1){ 	
		fscanf( fp,"%c%o\n",&character,&octal_value); 
		printf("Begin - %c, Octal - %o\n ", character,octal_value);   		

		if(feof(fp)){
			break; 
		}
			
		/* populate the memory */ 
		if(character == "@"){
			/* Indicates a change in memory pointer address - Due to branching */ 
	 		mem_pointer = octal_value;	// Ask Faust how memory allocation happens here 
		}
		else if(character == "*"){
			Reg[PC] = octal_value;
		}
		else{ 
	   		mem[mem_pointer] = octal_value & 0x00FF; 
	      		mem[mem_pointer+1] = (octal_value & 0xFF00) >> 8; 
		}
		mem_pointer = mem_pointer + 2;
 
	}
	fclose(fp); 
	return(0); 
}
	









 




