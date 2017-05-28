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
          Reg[PC] = 2; 
	  /* Add a command line if else - if the user does not specify a starting address, the starting address defaults to 0 */ 
	  
	  instruction_set  fetched_instruction; 
          long int instruction_counter = 0; 

	  /* Trace file generatin Initialization */ 
	  FILE * trace = fopen("tracefile.txt","w"); 
	  if(trace == NULL){
		printf("Error opening trace file!\n"); 
	        exit(1); 
	  } 

	  done = 0; // Ignores the Fetch > Decode > Execute for now 
	  while(!done){
		 
	   	// Fetch Instruction  
	  	fetched_instruction = (union instruction_set)read_mem(trace,INSTRUCTION_FETCH,Reg[PC]); 
	  	printf("Instruction Fetched !\n");
		// Decode Instruction  		
               /* Pseudo code */
		/* 
		
		IF fetched_instruction.ZOP is either HALT | WAIT | RESET |NOP
		ELSE IF fetched_instruction.OOP is between 0050 and 0067
		
		ELSE IF fetched_instruction.OHOP is between 070 and 074

		ELSE IF fetched_instruction.TOP is --- 

		ELSE IF 

		*/
		if(fetched_instruction.fetched == HALT){
			done = 1; 
			printf("Done is set to 1\n"); 
		} 
		else if(fetched_instruction.fetched == NOP){

		}
		else{
			switch((fetched_instruction.fetched & 0xF000)>> 12){
				/* If bits 15 thru 12 is true for the following case - We have the given scenerios of possible ISA inside the case statements */
				/* Matches with - One_half and SOB */
				case 07:
					/* We will look at bits 11 thru 9 to distinguish between one half operand and SOB */ 
					if(((fetched_instruction.fetched & 0x0E00) >> 9)== 07){
						printf("SOB Instruction\n");
					}
					else{
						printf("One Half Operand Instruction\n"); 

					}
					break; 
				
				/* Matches with - One operand and JSR and SWAB and RTS and PSW and Branch and other instructions */ 
				case 00:
					/* We will look at the 11th bit to determine between (JSR, One Op) or the rest */ 
					if((fetched_instruction.fetched & 0x0800) >> 11){
						
						/* If Bits 11 thru 9 follows the patter 0100 - True it is JSR, false it is a one operand */ 
						if(((fetched_instruction.fetched & 0x0E00)>> 9) == 04){			
							printf("JSR Instruction\n"); 
						}
						else{
							printf("One operand Instruction\n"); 

						}		
					}
					else{
						/* If bit 11 thru 6 is true it is SWAB operation or else it is (Other Instructions, Branch, RTS, PSW)  */
						if(((fetched_instruction.fetched & 0x0FC0) >> 6) == 03){
							printf("SWAB Operation\n"); 	

						}
						/* Else if bits 11 thru 8 is zero it is not zero it is a branch instruction otherwise (RTS | PSW) */
						else if(((fetched_instruction.fetched & 0x0F00) >> 8) != 0){
							printf("Branch Instruction\n"); 
							}
						else{
							if(((fetched_instruction.fetched & 0x0080) >> 7) == 01){
								if(((fetched_instruction.fetched & 0x00F8) >> 3) == 010){
									printf("RTS Instruction\n"); 
								}
								else{
									printf("PSW Instruction\n"); 
								}
							}
							else{
								printf("Other Instruction\n"); 
							}
						}
						
					}					
					break;  

				/* Matches with - Branch instructions */ 
				case 010:
					printf("Branch Instruction\n"); 
					break; 
				/* If none of the above sets are a match it is a two_operand instructions */ 
				default:
					printf("Two Operand Instruction\n"); 
			}
			
		}
		// Execute Instruction 
		 
	 
	
		// Increment 
	        Reg[PC] = Reg[PC] + 2; 	
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
	    	 
	/* Print everything in the memory */ /* The correct format specifier to print address is %p followed by type (void *) */ 
	for(int i = 0; i < sizeof(mem); i++){
	    fprintf(f,"-----------------------------------------\n"); 
	    fprintf(f,"|   Content - %o  |     Address - %p	| \n",(unsigned char)mem[i],i);
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
			mem_value = (mem[address+1] << 8) | mem[address];

		} 
		/* Read instruction set */ 
		else if(type == INSTRUCTION_FETCH){
			mem_value = (mem[address+1] << 8) | mem[address];	  
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
		
		mem[address] = data & 0000377; 
		mem[address+1] = (data & 0177400) >> 8; 
		/* Trace file generation */ 
		fprintf(trace,"%d %o\n",type,address); 
		return; 						
}

/* Effective Address Calculation */ 
unsigned short Effective_Address(FILE *trace, unsigned short mode, unsigned short source) {
	unsigned short x;
	switch(mode) {
		
		case R_DEF 			: return Reg[source];
							  break;
						 
		case AUTOINCR 		: return Reg[source];
					          Reg[source] = Reg[source]+2;
					          break;
								 
		case AUTOINC_DEF    : return read_mem( trace,DATA_READ,Reg[source]);
					          Reg[source] = Reg[source]+2;
					          break;
									  
		case AUTODECR       : Reg[source] = Reg[source] - 2;
					          return Reg[source];
					          break;
									
		case AUTODECR_DEF   : Reg[source] = Reg[source] - 2;
							  return read_mem(trace,DATA_READ,Reg[source]);
							  break;
									  
		case INDEX          : x = read_mem( trace,DATA_READ,Reg[PC]);
							  Reg[source] = Reg[source] + x;
							  Reg[PC] = Reg[PC] + 2;
							  return Reg[source];
							  break;
									  
		case INDEX_DEF      : x = read_mem( trace,DATA_READ,Reg[PC]);
							  Reg[source] = Reg[source] + x;
							  Reg[PC] = Reg[PC] + 2;
   							  return read_mem(trace,DATA_READ, Reg[source]);
   							  break;
   								  
		default             : return source; 
							  printf("Following mode doesn't require Effective Address calculation\n");
							  break;
									  
	}
}

/* Register Read Function */ 
/* Returns the 16 bit data in the Registers */ 
unsigned short reg_READ(FILE *trace, unsigned short mode, unsigned short source) {
	unsigned short temp;
	switch(mode) {
		case REG			: return Reg[source]; 
							  break;
	
		default 			: temp = Effective_Address(trace, mode, source);
							  return read_mem(trace,DATA_READ,temp); 
							  break;									  
	}
}

/* Register Write Function */ 
/* Write the 16 bit data in the Registers */ 
void reg_WRITE(FILE *trace, unsigned short mode, unsigned short destination,unsigned short data) {
	unsigned short temp;
	/* Only if it is in Reg mode, we write to the register destination otherwise we write back 
	   to the memory to be read later for other addressing mode types */ 
	switch(mode) {
		case REG			: Reg[destination] = data;   
							  break;
	
		default 			: temp = Effective_Address(trace, mode, destination);
							  write_mem(trace,DATA_WRITE,temp,data); 
							  break;									  
	}
}





/* Double Operand Instruction Function */ 
void func_doubleoperand(FILE *trace, instruction_set input_var){
	unsigned short result;	
	unsigned short temp1;	
	unsigned short temp2;	
	
	switch(input_var.TOP.Opcode){
		case MOV: 	result = reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
					// PSW = 0; 
					break; 
				  
		case CMP: 	temp1 = reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					temp2 = reg_READ(trace,input_var.TOP.Modes_D,input_var.TOP.Destination); 
					result = temp1 + ~temp2 + 1;
					
					/* Flags not done yet */ 
		
					// PSW = 0; 
					// if(temp == 0){
							// PSW = Z; 
					// }
					
					// if(temp < 0){
							// PSW = PSW + N; 
					// }
					
					// if(!(temp && 0100000) >> 15){
							// PSW = PSW + C;		
					// }
					
					/* not sure yet about sign overflow */ 
					break;
					
		case BIT: 	/* BIT Test */ 
					/* Computes dest & src */ 
					/* NOT STORED */
					temp1 = reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					temp2 = reg_READ(trace,input_var.TOP.Modes_D,input_var.TOP.Destination);					
					result = temp1 ^ temp2; 
					
					// PSW = C; 					
					break;
					
		case BIC: 	/* Bit Clear */
					temp1 = ~reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					temp2 = reg_READ(trace,input_var.TOP.Modes_D,input_var.TOP.Destination);
					result = temp1 & temp2; 
					reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
					
					
					// PSW = C;						
					break;
					
		case BIS: 	/* Bit set,a.k.a logical OR */
					temp1 = ~reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					temp2 = reg_READ(trace,input_var.TOP.Modes_D,input_var.TOP.Destination);
					result = temp1 | temp2; 
					reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);

					// PSW = C; 
					break;
		
		case ADD:   /* Add */ 
					temp1 = reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					temp2 = reg_READ(trace,input_var.TOP.Modes_D,input_var.TOP.Destination);
					result = temp1 + temp2; 
					reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);

					// PSW = C; 
					/* SET FLAGS */
					break;
					
		case SUB: 	/* SUB */
					temp1 = reg_READ(trace,input_var.TOP.Modes_S,input_var.TOP.source);
					temp2 = reg_READ(trace,input_var.TOP.Modes_D,input_var.TOP.Destination);
					result = temp1 - temp2; 
					reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
					
					/* SET FLAGS */ 
					break; 
	}
}

/* One Half Intruction Function - Some additional two-operand require source operand */ 
void func_onehalfoperand(FILE *trace, instruction_set input_var){
	unsigned short temp1;
	unsigned short temp2;
	unsigned int result; 
	

	/* WORK IN PROGRESS */ 
	switch(input_var.OHOP.Opcode){
		case MUL:	temp1 = reg_READ(trace,input_var.OHOP.Mode,input_var.OHOP.Source);
					result = Reg[input_var.OHOP.Register] * temp1; 
					
					write_mem(); 
					break;
					
		case DIV: 	/* Divide */ 
					
					break;
					
		case ASH:   /* Arithmetic shift */ 
					
					break;

		case ASHC:  /* Arithmetic shift combined */ 
					
					break;
					
		case XOR:   /* Exclusive OR */ 
					
					break;

					
		case SOB:   /* Subtract one and branch */ 
					
					break;  		
	}
}



int open_file(char *arr){ 
	/* Opens the filename pointed to - 'r' opens a file for reading */ 
 	FILE * fp = fopen(arr,"r"); 
	char character;
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
		//printf("Begin - %c, Octal - %o\n ", character,octal_value);   		

		if(feof(fp)){
			break; 
		}
			
		/* populate the memory */ 
		/* "@" gives a warning - should '@'. In C single quoutes delimits a single character whereas double qoutes are for strings */ 
		if(character == '@'){
			/* Indicates a change in memory pointer address - Due to branching */ 
	 		mem_pointer = octal_value;	// Ask Faust how memory allocation happens here 
		}
		else if(character == '*'){
			Reg[PC] = octal_value;
		}
		else{ 
	   		mem[mem_pointer] = octal_value & 0000377;
			printf("Mem Value - %o, Mem Index - %d\n",mem[mem_pointer], mem_pointer);
			mem[mem_pointer+1] = (octal_value & 0177400) >> 8; 
			printf("Mem Value - %o, Mem Index - %d\n",mem[mem_pointer+1], mem_pointer+1); 
			printf("Final Value - %o\n", (mem[mem_pointer+1] << 8) | (mem[mem_pointer]));  
			mem_pointer = mem_pointer + 2; 
		}
	
 
	}
	fclose(fp); 
	return(0); 
}
	









 




