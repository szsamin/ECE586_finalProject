/* Simulator C file 
   Author: Apurva Gandole, Chandan Muralidhar, Shadmna Samin, Sourabh Balakrishna
  
   Description: 

*/ 

// Library Declarations 
#include "../Code/simulator.h" 
#include <stdlib.h>

// Global data type initializations
char dataStr[MEM];  				
unsigned char mem[MEM]; 		// Memory size declaration 
unsigned short int Reg [8]; 		// Register size declaration 

int main(int argc,int argv){
	  /* initial step read file */ 
	  char *Source = "../TestFiles/source.ascii";   
	  open_file(Source); 
	  
	  int done = 0;

	  /* Initial Starting address */ 
          Reg[PC] = 8; 
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
		
		// Step by Step Execution 
		// Waits for Enter Key press	
		getchar();	
 
	   	// Fetch Instruction  
	  	fetched_instruction = (union instruction_set)read_mem(trace,INSTRUCTION_FETCH,Reg[PC]); 
		Reg[PC] += 2; 	  	
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
							func_singleoperand(trace,fetched_instruction);
						}		
					}
					else{
						/* If bit 11 thru 6 is true it is SWAB operation or else it is (Other Instructions, Branch, RTS, PSW)  */
						if(((fetched_instruction.fetched & 0x0FC0) >> 6) == 03){
							printf("SWAB Operation\n");
							func_singleoperand(trace,fetched_instruction);  	

						}
						/* Else if bits 11 thru 8 is zero it is not zero it is a branch instruction otherwise (RTS | PSW) */
						else if(((fetched_instruction.fetched & 0x0F00) >> 8) != 0){
							printf("Branch Instruction\n");
							func_conditionalbranch(trace,fetched_instruction); 
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
					func_conditionalbranch(trace,fetched_instruction); 
					break; 
				/* If none of the above sets are a match it is a two_operand instructions */ 
				default:
					printf("Two Operand Instruction\n");
					printf("Instruction Fetched - %o\n",fetched_instruction.fetched);
					printf("B %o, Opcode %o, Mode Source %o, Source %o, Mode Destination %o, Destination %o\n",fetched_instruction.TOP.B,fetched_instruction.TOP.Opcode,fetched_instruction.TOP.Mode_S,fetched_instruction.TOP.Source,fetched_instruction.TOP.Mode_D,fetched_instruction.TOP.Destination);
					func_doubleoperand(trace,fetched_instruction);  
			}
			
		}
		// Execute Instruction 
		print_REG();  
	 
		
	
	        
		   	
	 }

	  /* Throw data into mem */ 
			
	  /* Display function */ 
	  display(); 

	  /* Trace file end */ 
	  fclose(trace); 
	  return(0); 
}

void print_REG(){
	// Step by step Print all the registers and affected Flags //
	printf("-------------------- NEW INSTRUCTION ----------------\n");
	printf("Reg 0 - - - - %o\n",Reg[R0]); 
	printf("Reg 1 - - - - %o\n",Reg[R1]);
	printf("Reg 2 - - - - %o\n",Reg[R2]); 
	printf("Reg 3 - - - - %o\n",Reg[R3]);
	printf("Reg 4 - - - - %o\n",Reg[R4]);
	printf("Reg 5 - - - - %o\n",Reg[R5]); 
	printf("SP - - - - -  %o\n",Reg[SP]);
	printf("PC - - - - -  %o\n",Reg[PC]); 
	printf("=================FLAGS==============================\n");
	printf("Carry C - -  %o\n",psw.C); 
	printf("Overflow V - %o\n",psw.V);
	printf("Zero Z - - - %o\n",psw.Z);
	printf("Negative N - %o\n",psw.N);
	printf("------------------- END OF INSTRUCTION --------------\n");
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
	    fprintf(f,"|   Content - %o  |     Address - %d	| \n",(unsigned char)mem[i],i);
	    fprintf(f,"-----------------------------------------\n"); 
	}
	
	fclose(f); 
	return;  
}

/* Read Memory Function */
/* Return value from read_mem - either instruction or memory content will be 16 bit return value */  
unsigned short read_mem(FILE *trace,unsigned short type, unsigned short address){
		
		unsigned short mem_value; 
		printf("Address being read from - %o\n",address); 		

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
		printf("DONE READING - %o\n",mem_value); 
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
	
	if(source == PC){ 
		switch(mode){
			case IMD    : printf("Immediate Addressing\n"); 
				      printf("Reg[PC] = %o\n",Reg[PC]);
				      x = Reg[PC];
				      Reg[PC] = x + 2;
				      printf("This shit works\n"); 
				       
				      printf("New effective address - %o\n",x); 
				      return x;
				      break;

			case RLDIR  : printf("Relative Direct addressing\n");
				      x = read_mem(trace,DATA_READ,Reg[PC]);
				      Reg[PC] = Reg[PC] + 2;	
				      return x + Reg[PC]; 	
 				      break; 		

			case ABSDR  : printf("Absolute Direct \n"); 
				      x = read_mem(trace,DATA_READ,Reg[PC]);
			 	      Reg[PC] = Reg[PC] + 2;
					  return x; 
				      break;

			case RLINDIR: printf("Relative Indirect addressing\n");
				      x = read_mem(trace,DATA_READ,Reg[PC]);
				      Reg[PC] = Reg[PC] + 2; 
				      unsigned short y = x + Reg[PC];
				      return read_mem(trace,DATA_READ,y);
				      break;
		}
	}
	else{
	switch(mode) {
		
		case R_DEF 	    : return Reg[source];
				      break;
						 
		case AUTOINCR       :
				      return Reg[source];
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
}

/* Register Read Function */ 
/* Returns the 16 bit data in the Registers */ 
signed short reg_READ(FILE *trace, unsigned short mode, unsigned short source) {
	unsigned short temp;
	printf("READING FROM REG\n"); 
	switch(mode) {
		case REG			: temp =  Reg[source];
						    
						  break;
	
		default 			: temp = Effective_Address(trace, mode, source);
						  
						  temp =  read_mem(trace,DATA_READ,temp); 
					 	  break;									  
	}
	printf("DONE READING FROM REG\n"); 	
	return temp; 
}

/* Register Write Function */ 
/* Write the 16 bit data in the Registers */ 
void reg_WRITE(FILE *trace, unsigned short mode, unsigned short destination,signed short data) {
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
	signed int result;	
	/* temps used for comparing */
	signed short temp1;	
	signed short temp2;	
	
	switch(input_var.TOP.Opcode){
		case MOV: 
			  printf("MOV INSTRUCTION\n");  
			  result = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
			  if(result == 0){
			  	psw.Z = 1; 
			  }else{psw.Z = 0;}

			  if(result < 0){
				psw.N = 1; 
			  }else{psw.N = 0;}

			  psw.V = 0;
			  printf("DONE WITH MOV INSTRUCTION -SHOULD FETCH A NEW INSTRUCTION\n"); 
			  break; 
				  
		case CMP:
			  printf("CMP Instruction\n");		
			  temp1 = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination); 
			  result = temp1 + ~temp2 + 1;
					
			  if(result == 0){  		   
			  	psw.Z = 1; 		
			  }else{psw.Z = 0;} 		
	   	
			  if(result < 0){
				psw.N = 1;  		  /* not sure yet about sign overflow */ 
			  }else{psw.N = 0;} 
 
			  psw.C = !((result & 020000) >> 15); 
			  /* Check for overflow bit - if source MSB XOR destination MSB is true, also negate of destination MSB XOR result MSB is true, then overflow must be true */ 
		          psw.V = (((temp1 & 0200000)>> 15) ^ (( temp2 & 020000 >> 15))) && !(((temp2 & 020000) >> 15) && (result & 020000) >> 15); 	
			

			  break;
					
		case BIT: /* BIT Test */ 
			  /* Computes dest & src */ 
			  /* NOT STORED */
			  printf("BIT Instruction\n");
			  temp1 = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination);					
			  result = temp1 ^ temp2; 
			  if(result == 0){
			  	psw.Z = 1; 
			  }else{psw.Z = 0;}

			  if(result < 0){
				psw.N = 1; 
			  }else{psw.N = 0;}
 
			  psw.V = 0;						
			  break;
					
		case BIC: /* Bit Clear */
			  printf("BIC Instruction\n");
			  temp1 = ~reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination);
			  result = temp1 & temp2; 
			  reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
					
			  if(result == 0){
			  	psw.Z = 1; 
			  }else{psw.Z = 0;}

			  if(result < 0){
				psw.N = 1; 
			  }else{psw.N = 0;}

			  psw.V = 0; 
										
			  break;
					
		case BIS: /* Bit set,a.k.a logical OR */
			  printf("BIS Instruction\n");
			  temp1 = ~reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination);
			  result = temp1 | temp2; 
			  reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
			  
			  if(result == 0){
			  	psw.Z = 1; 
			  }else{psw.Z = 0;}

			  if(result < 0){
				psw.N = 1; 
			  }else{psw.N = 0;}

			  psw.V = 0;  
			  break;
		
		case ADDSUB:   /* Add */
		  	if(input_var.TOP.B == 0){
			  	printf("ADD Instruction\n");				
				temp1 = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
				temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination);
				result = temp1 + temp2; 
				reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
			        if(result == 0){
			              psw.Z = 1; 
				}else{psw.Z = 0;}

			        if(result < 0){
				      psw.N = 1; 
			        }else{psw.N = 0;}
			 	 psw.C = ((result & 020000) >> 15); 
			 	 /* Check for overflow bit - if the negate of source MSB XOR destination MSB is true, also destination MSB XOR result MSB is true, then overflow must be true */ 
		          	psw.V = !(((temp1 & 0200000)>> 15) ^ (( temp2 & 020000 >> 15))) && (((temp2 & 020000) >> 15) && (result & 020000) >> 15);
				break; 	
				}
			else{	
			 	/* SUB */
				printf("SUB Instruction\n");
				temp1 = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
				temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination);
				result = temp2 + ~temp1 + 1; 
				reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
			  	if(result == 0){
			  	psw.Z = 1; 
			 	 }else{psw.Z = 0;}

			  	if(result < 0){
				      psw.N = 1; 
			  	}else{psw.N = 0;}

			 	 psw.V = 0; 
				
			  	psw.C = !((result & 020000) >> 15); 
			  	/* Check for overflow bit - if source MSB XOR destination MSB is true, also negate of destination MSB XOR result MSB is true, then overflow must be true */ 
		         	 psw.V = (((temp1 & 0200000)>> 15) ^ (( temp2 & 020000 >> 15))) && !(((temp2 & 020000) >> 15) && (result & 020000) >> 15); 	
				} 
				break; 
	}
}

void func_singleoperand(FILE *trace,instruction_set input_var){
	signed short result;
	signed short temp1; 
	signed short temp2; 	


	switch(input_var.OOP.Opcode){
		
		case CLR:
			  printf("CLR Instruction\n");		
				reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,0); 
			  psw.Z = 1;
			  psw.N = 0;
			  psw.C = 0;
			  psw.V = 0; 

			  break;

		case INC: 
			  printf("INC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 + 1; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = (temp1 == 0077777) ? 1 : 0; 
			  
			  break;

		case DEC: 
			  printf("DEC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 - 1; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 
			  printf("DEC Instruction"); 
  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.C = (temp1 == 0100000) ? 1:0;  
	
			  break;
		case ADC: 
			  printf("ADC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 + psw.C; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = ((temp1 == 0077777) && (psw.C)) ? 1 : 0; 
			  psw.C = ((temp1 == 0177777) && (psw.C)) ? 1 : 0; 

			  break;

		case SBC: 
			  printf("SBC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 - psw.C; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = (temp1 == 0100000) ? 1 : 0; 
			  psw.C = ((result == 0) && (psw.C)) ? 0 : 1; 
	
			  break;
		case TST: 
			  printf("TST Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = 0-temp1; 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  
			  psw.V = 0;
			  psw.C = 0; 

			  break;

		case NEG: 
			  printf("NEG Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = -temp1; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = (temp1 == 0100000) ? 1 : 0; 
			  psw.C = (result == 0) ? 0 : 1; 

			  break;
		case COM: 
			  printf("COM Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = ~temp1; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = 0; 
			  psw.C = 1; 

			  break;
		case ROR: 
			  printf("ROR Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  temp2 = (temp1 >> 1) | (psw.C << 15) ;
			  psw.C = (temp1 & 0000001); 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = psw.N ^ psw.C;
			  break;
		case ROL: 
			  printf("ROL Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  temp2 = (temp1 << 1) | (psw.C) ;
			  psw.C = (temp1 & 0100000); 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = psw.N ^ psw.C; 
			  break;
		case ASR: 
			  printf("ASR Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = (temp1 & 0100000) | (temp1 >> 1);
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0;  
			  psw.C = (temp1 & 0000001); 
			  psw.V = psw.N ^ psw.C; 
			  break;
		case ASL: 
			  printf("ASL Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = (temp1 & 0000001) | (temp1 << 1); 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.C = (temp1 & 0100000);
			  psw.V = psw.N ^ psw.C; 
			
			  break;
		case SWAB:
			  printf("SWAB Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = ((temp1 && 0xff00) >> 8) | ((temp1 && 0x00ff) << 8);			  
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = ((result & 0x00ff) == 0 ) ? 1 : 0;
			  psw.N = ((result & 0x0080) >> 7) ? 1 : 0; 
			  psw.V = 0; 
			  psw.C = 0;
			  break;
		case SXT:
			  printf("SXT Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = (psw.N == 1) ? 0177777 : 0000000; 
			  reg_WRITE(trace,input_var.OOP.Mode,input_var.OOP.Register,result); 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = 0; 
			  psw.C = 1; 


			  break;
	}
}

void func_conditionalbranch(FILE *trace,instruction_set input_var){
	unsigned short temp;
	unsigned short result; 
	temp = read_mem(trace,DATA_READ,Reg[PC]);
	result = temp + 2*input_var.BRANCH.Offset; 	

	if(input_var.BRANCH.x){
		switch(input_var.BRANCH.Opcode){
			case BPL :  if(psw.N == 0){
					Reg[PC] = result;}
				    break;
						
			case BMI :  if(psw.N == 1){
					Reg[PC] = result; 
					}
				    break;
						
			case BVC : if(psw.V == 0){
					Reg[PC] = result;}
				   break; 
						
			case BVS : if(psw.V == 1){
					Reg[PC] = result;} 
				   break;
			/*				
			case BHIS: if(C == 0){
							Reg[PC] = result; 
					   }
						break; 
			*/			
			case BCC : if(psw.C == 0){
				        Reg[PC] = result;}
				   break;
			/*			
			case BLO : if(C == 1){
							Reg[PC] = result; 
					   }
						break;
			*/			
			case BCS : if(psw.C == 1){
					Reg[PC] = result;}
				   break; 
						
			case BHI : if((psw.C | psw.Z) == 0){
					Reg[PC] = result;}
				   break;
						
			case BLOS: if((psw.C | psw.Z) == 1){
					Reg[PC] = result;}
				   break; 
						
		}
	}
	else{
		switch(input_var.BRANCH.Opcode){
			case BR :   Reg[PC] = result;
				    printf("BR Branch\n");
	               		    break;  
						
			case BNE:   if(psw.Z == 0){
					Reg[PC] = result;}
				    break;
						
			case BEQ:   if(psw.Z == 1){
					Reg[PC] = result;}
				    break; 
			/*			
			case BEG:   if( N ^ V == 0){
							Reg[PC] = result; 
						}
						break; 
			*/			
			case BLT:   if( psw.N ^ psw.Z == 1){
					Reg[PC] = result;}
				    break; 
						
			case BGT:   if( (psw.Z | (psw.N ^ psw.V)) == 0){
					Reg[PC] = result;}
				    break;
						
			case BLE:   if( (psw.Z | (psw.N ^ psw.V)) == 1){
					Reg[PC] = result;}
				    printf("BLE Branch\n");
		   		    break;
		}
	}
}


 // STILL NEED TO FIGURE IF WE NEED ONE HALF OPERATION 

// /* One Half Intruction Function - Some additional two-operand require source operand */ 
// void func_onehalfoperand(FILE *trace, instruction_set input_var){
	// unsigned short temp1;
	// unsigned short temp2;
	// unsigned int result; 
	

	// /* WORK IN PROGRESS */ 
	// switch(input_var.OHOP.Opcode){
		// case MUL:	temp1 = reg_READ(trace,input_var.OHOP.Mode,input_var.OHOP.Source);
					// result = Reg[input_var.OHOP.Register] * temp1; 
					
					// write_mem(); 
					// break;
					
		// case DIV: 	/* Divide */ 
					
					// break;
					
		// case ASH:   /* Arithmetic shift */ 
					
					// break;

		// case ASHC:  /* Arithmetic shift combined */ 
					
					// break;
					
		// case XOR:   /* Exclusive OR */ 
					
					// break;

					
		// case SOB:   /* Subtract one and branch */ 
					
					// break;  		
	// }
// }


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
	









 




