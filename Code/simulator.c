// Library Declarations 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 


// DEFINES
#define byte_length 4 
#define KB 1024
#define MB (KB*KB) 
#define MEM (64*KB) 

// Global data type initializations
char dataStr[MEM];  				
unsigned char mem[MEM]; 		// Memory size declaration 
unsigned short int Reg [8]; 		// Register size declaration 
unsigned short int PSW; 		// 

// Function prototypes
int open_file(char *arr);
void display();  

int main(){
          /* initial step read file */ 
	  char *Source = "../TestFiles/source.ascii";   
	  open_file(Source); 

	  /* Throw data into mem */ 

	  /* Display function */ 
	  display(); 

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



int open_file(char *arr){ 
	/* Opens the filename pointed to - 'r' opens a file for reading */ 
 	FILE * fp = fopen(arr,"r"); 
	char *character;
        int scan_file;
	unsigned int octal_value;
        unsigned short int mem_pointer = 0;

	/* Check if the file exits */ 
	if(fp == NULL){
		perror("Error in opening file"); 
		return(-1); 
	}	
        Reg[7] = 0;
	
	while(1){ 	
		fscanf( fp,"%c%o\n",&character,&octal_value); 
		printf("Begin - %c, Octal - %o\n ", character,octal_value);  
		if(feof(fp)){
			break; 
		}

		/* populate the memory */ 
		if(character == "@"){
		   mem_pointer = 0; 
		}
		else if(character == "-"){
		   Reg[7] = octal_value; 
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
	
	/* Working - DO NOT REMOVE 
	while(1){ 
		c = fgetc(fp); 
		if(feof(fp)){
			break; 
		}
		printf("%c",c); 
	}

*/  




