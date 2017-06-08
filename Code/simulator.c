/* Simulator C file 
   Author: Apurva Gandole, Chandan Muralidhar, Shadmna Samin, Sourabh Balakrishna
  
   Description: 
*/ 

/* DEBUG PRINT STATEMENTS */ 
//#define DEBUG 


// Library Declarations 
#include "../Code/simulator.h" 
#include <stdlib.h>

// Global data type initializations
char dataStr[MEM];  				
unsigned char mem[MEM]; 		// Memory size declaration 
unsigned short Reg [8]; 		// Register size declaration 

unsigned short current_EA;		// Current global EA available to all functions for manipulation  




int main(void){
	  /* initial step read file */ 
	  char *Source = "../TestFiles/source.ascii";   
	  open_file(Source); 
	  
	  int done = 0;
	  int input; 

	  /* Initial Starting address */
	  printf("Input starting program counter value\n");
	  scanf("%d",&input); 
	  if(input == NULL){
		printf("Valid Program counter not entered\n"); 
		exit(1); 
	  } 
          Reg[PC] = input; 
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
		Reg[PC] = Reg[PC] +  2; 	  	
		instruction_counter++;
		#ifdef DEBUG 
			printf("INSTRUCTION COUNTER = %d\n",instruction_counter);  	
			printf("Instruction Fetched !\n");
		#endif
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
						#ifdef DEBUG 
							printf("SOB Instruction\n");
						#endif
						printf("SOB Instruction - one half operand - NOT IMPLEMENTED in this simulator\n");
					}
					else{
						#ifdef DEBUG
							printf("One Half Operand Instruction\n"); 
						#endif
						printf("One HALF operand Instruction - NOT IMPLEMENT in this simulator\n"); 
					}
					break; 
				
				/* Matches with - One operand and JSR and SWAB and RTS and PSW and Branch and other instructions */ 
				case 00:
					/* We will look at the 11th bit to determine between (JSR, One Op) or the rest */ 
					if((fetched_instruction.fetched & 0x0800) >> 11){
						
						/* If Bits 11 thru 9 follows the patter 0100 - True it is JSR, false it is a one operand */ 
						if(((fetched_instruction.fetched & 0x0E00)>> 9) == 04){
							#ifdef DEBUG			
								printf("JSR Instruction\n");
							#endif 
							func_jump(trace,fetched_instruction); 
						}
						else{
							#ifdef DEBUG
								printf("One operand Instruction\n"); 
							#endif
							func_singleoperand(trace,fetched_instruction);
						}		
					}
					else{
						/* If bit 11 thru 6 is true it is SWAB operation or else it is (Other Instructions, Branch, RTS, PSW)  */
						if(((fetched_instruction.fetched & 0x0FC0) >> 6) == 03){
							#ifdef DEBUG
								printf("SWAB Operation\n");
							#endif
							func_singleoperand(trace,fetched_instruction);  	

						}
						/* Else if bits 11 thru 8 is zero it is not zero it is a branch instruction otherwise (RTS | PSW) */
						else if(((fetched_instruction.fetched & 0x0F00) >> 8) != 0){
							#ifdef DEBUG
								printf("Branch Instruction\n");
							#endif
							func_conditionalbranch(trace,fetched_instruction); 
							}
						else{
							if(((fetched_instruction.fetched & 0x0080) >> 7) == 01){
								if(((fetched_instruction.fetched & 0x00F8) >> 3) == 010){
									#ifdef DEBUG
										printf("RTS Instruction\n");
									#endif
									func_jump(trace,fetched_instruction);  
								}
								else{
									#ifdef DEBUG
									printf("PSW Instruction\n"); 
									#endif
									func_psw(trace,fetched_instruction); 
				
								}
							}
							else{
								#ifdef DEBUG
								printf("JUMP Instruction\n"); 
								#endif
								func_jump(trace,fetched_instruction); 
							}	
						}
						
					}					
					break;  

				/* Matches with - Branch instructions */ 
				case 010:
					#ifdef DEBUG
						printf("Branch Instruction\n");
					#endif
					func_conditionalbranch(trace,fetched_instruction); 
					break; 
				/* If none of the above sets are a match it is a two_operand instructions */ 
				default:
					#ifdef DEBUG
						printf("Two Operand Instruction\n");
						printf("Instruction Fetched - %o\n",fetched_instruction.fetched);
						printf("B %o, Opcode %o, Mode Source %o, Source %o, Mode Destination %o, Destination %o\n",fetched_instruction.TOP.B,fetched_instruction.TOP.Opcode,fetched_instruction.TOP.Mode_S,fetched_instruction.TOP.Source,fetched_instruction.TOP.Mode_D,fetched_instruction.TOP.Destination);
					#endif
					func_doubleoperand(trace,fetched_instruction);  
			}
			
		}
		// Execute Instruction 
		print_REG(); 
		
		#ifdef DEBUG
		printf("OLD - %o\n",read_mem(trace,DATA_READ,0)); 
		printf("NEW - %o\n",read_mem(trace,DATA_READ,2)); 
		printf("FIBO - %o\n",read_mem(trace,DATA_READ,4)); 
		printf("N - %o\n",read_mem(trace,DATA_READ,6)); 
	 	#endif	
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
		#ifdef DEBUG
			printf("Address being read from - %d\n",address); 		
		#endif

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
		fprintf(trace,"%d %d\n",type,address); 
		#ifdef DEBUG
			printf("DONE READING - %o\n",mem_value); 
		#endif
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
			case IMD    : 
				      #ifdef DEBUG
				      printf("Immediate Addressing\n"); 
				      printf("Reg[PC] = %o\n",Reg[PC]);
				      #endif
				      x = Reg[PC];
				      Reg[PC] = x + 2;
					
				      #ifdef DEBUG
				      printf("This shit works\n"); 
				      printf("New effective address - %o\n",x);
				      #endif 
				      return x;
				      break;

			case RLDIR  : 
				      #ifdef DEBUG	
				      printf("Relative Direct addressing\n");
				      printf("REG[PC] before read_mem - %o\n",Reg[PC]); 
				      #endif

				      x = read_mem(trace,DATA_READ,Reg[PC]);
				      Reg[PC] = Reg[PC] + 2;
				      #ifdef DEBUG
				      printf("Value of X - %o\n",x); 
				      printf("Value of X + Reg[PC] - %o\n",x + Reg[PC]);
				      #endif	 	
				      return x + Reg[PC]; 	
 				      break; 		

			case ABSDR  : 
				      #ifdef DEBUG 
				      printf("Absolute Direct \n");
				      #endif 
				      x = read_mem(trace,DATA_READ,Reg[PC]);
			 	      Reg[PC] = Reg[PC] + 2;
					  return x; 
				      break;

			case RLINDIR: 
				      #ifdef DEBUG
				      printf("Relative Indirect addressing\n");
				      #endif
				      x = read_mem(trace,DATA_READ,Reg[PC]);
				      Reg[PC] = Reg[PC] + 2; 
				      unsigned short y = x + Reg[PC];
				      return read_mem(trace,DATA_READ,y);
				      break;
		}
	}
	else{
	switch(mode) {
		
		case R_DEF 	    : 
				      #ifdef DEBUG 
				      	printf("Register Deferred\n"); 
				      #endif
				      return Reg[source];
				      break;
						 
		case AUTOINCR       :
				      #ifdef DEBUG 
					printf("Auto Increment\n");
				      #endif 
				      return Reg[source]; 
				      Reg[source] = Reg[source]+2;
				      break;
								 
		case AUTOINC_DEF    : 
				      #ifdef DEBUG 
				      	printf("Auto Increment Deferred\n");
				      #endif 
				      return read_mem( trace,DATA_READ,Reg[source]);
				      Reg[source] = Reg[source]+2;
				      break;
									  
		case AUTODECR       : 
				      #ifdef DEBUG 
				      	printf("Auto Decrement\n");
				      #endif 
				      Reg[source] = Reg[source] - 2;
				      return Reg[source];
				      break;
									
		case AUTODECR_DEF   : 
				      #ifdef DEBUG 
					printf("Auto Decrement Deferred\n"); 
				      #endif
				      Reg[source] = Reg[source] - 2;
				      return read_mem(trace,DATA_READ,Reg[source]);
				      break;
									  
		case INDEX          : 
				      #ifdef DEBUG
					printf("Index Addressing\n"); 
				      #endif
				      x = read_mem( trace,DATA_READ,Reg[PC]);
				      Reg[source] = Reg[source] + x;
				      Reg[PC] = Reg[PC] + 2;
				      return Reg[source];
				      break;
									  
		case INDEX_DEF      : 
				      #ifdef DEBUG
					printf("Index Deferred\n");
				      #endif
				      x = read_mem( trace,DATA_READ,Reg[PC]);
				      Reg[source] = Reg[source] + x;
				      Reg[PC] = Reg[PC] + 2;
   				      return read_mem(trace,DATA_READ, Reg[source]);
   				      break;
   								  
		default             : 
				      #ifdef DEBUG 
				      printf("Register Mode\n");
				      #endif
				      return source; 
				      break;
									  
	}
	}
}

/* Register Read Function */ 
/* Returns the 16 bit data in the Registers */ 
signed short reg_READ(FILE *trace, unsigned short mode, unsigned short source) {
	unsigned short temp;
	#ifdef DEBUG 
	printf("READING FROM REG\n");
	#endif 
	switch(mode) {
		case REG			: temp =  Reg[source];
						  #ifdef DEBUG 
						  printf("Value stored in temp : %o\n",Reg[source]);
						  #endif  
						  break;
	
		default 			: temp = Effective_Address(trace, mode, source);
						  current_EA = temp; 			  
						  temp =  read_mem(trace,DATA_READ,temp); 
					 	  break;									  
	}
	#ifdef DEBUG 
	printf("DONE READING FROM REG\n");
	#endif 	
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
	
	unsigned short Mode_D = input_var.TOP.Mode_D; 	
	switch(input_var.TOP.Opcode){
		case MOV: 
			  #ifdef DEBUG 
			  printf("MOV INSTRUCTION\n");
			  #endif  
			  result = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);
			  if(result == 0){
			  	psw.Z = 1; 
			  }else{psw.Z = 0;}

			  if(result < 0){
				psw.N = 1; 
			  }else{psw.N = 0;}

			  psw.V = 0;
			   
			  break; 
				  
		case CMP: 
			  #ifdef DEBUG
			  printf("CMP Instruction\n");		
			  #endif
			  temp1 = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
			  temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination); 
			  result = temp1 - temp2;
			  #ifdef DEBUG
		          printf("RESULT FOR CMP = %d\n",result); 			
			  #endif
			  if(result == 0){  		   
			  	psw.Z = 1; 		
			  }else{psw.Z = 0;} 		
	   	
			  if(result < 0){
				psw.N = 1;  		  /* not sure yet about sign overflow */ 
			  }else{psw.N = 0;} 
 
			  psw.C = !((result & 0200000) >> 16); 
			  /* Check for overflow bit - if source MSB XOR destination MSB is true, also negate of destination MSB XOR result MSB is true, then overflow must be true */ 
		          psw.V = (((temp1 & 0100000)>> 15) ^ ((( temp2 & 0100000) >> 15))) && !(((temp2 & 0100000) >> 15) ^ ((result & 0100000) >> 15)); 	
			

			  break;
					
		case BIT: /* BIT Test */ 
			  /* Computes dest & src */ 
			  /* NOT STORED */
			  #ifdef DEBUG 
			  printf("BIT Instruction\n");
			  #endif 
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

			  /* only for instruction that reuse destination */
			  if(Mode_D != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);}
					
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
			  /* only for instruction that reuse destination */
			  if(Mode_D != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);}
			  
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
				/* only for instruction that reuse destination */				 
			 	 if(Mode_D != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			 	 else{reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);}
			        if(result == 0){
			              psw.Z = 1; 
				}else{psw.Z = 0;}

			        if(result < 0){
				      psw.N = 1; 
			        }else{psw.N = 0;}
			 	 psw.C = ((result & 0200000) >> 16); 
			 	 /* Check for overflow bit - if the negate of source MSB XOR destination MSB is true, also destination MSB XOR result MSB is true, then overflow must be true */ 
		          	psw.V = !(((temp1 & 0100000)>> 15) ^ (( temp2 & 0100000 >> 15))) && (((temp2 & 0100000) >> 15) && (result & 0100000) >> 15);
				break; 	
				}
			else{	
			 	/* SUB */
				printf("SUB Instruction\n");
				temp1 = reg_READ(trace,input_var.TOP.Mode_S,input_var.TOP.Source);
				temp2 = reg_READ(trace,input_var.TOP.Mode_D,input_var.TOP.Destination);
				result = temp2 - temp1; 
				
				 
			  	/* only for instruction that reuse destination */
			  	if(Mode_D != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  	else{reg_WRITE(trace,input_var.TOP.Mode_D, input_var.TOP.Destination, result);}
				
			  	if(result == 0){
			  	psw.Z = 1; 
			 	 }else{psw.Z = 0;}

			  	if(result < 0){
				      psw.N = 1; 
			  	}else{psw.N = 0;}

			 	 psw.V = 0; 
				
			  	psw.C = !((result & 0200000) >> 16); 
			  	/* Check for overflow bit - if source MSB XOR destination MSB is true, also negate of destination MSB XOR result MSB is true, then overflow must be true */ 
		         	 psw.V = (((temp1 & 0100000)>> 15) ^ (( temp2 & 0100000 >> 15))) && !(((temp2 & 0100000) >> 15) && (result & 0100000) >> 15); 	
				} 
				break; 
	}
}

void func_singleoperand(FILE *trace,instruction_set input_var){
	signed short result;
	signed short temp1; 
	signed short temp2; 	

	unsigned short Mode = input_var.OOP.Mode; 
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
			  
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
			  write_mem(trace,DATA_WRITE,current_EA,result); 
			   

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = (temp1 == 0077777) ? 1 : 0; 
			  
			  break;

		case DEC: 
			  printf("DEC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 - 1; 
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);} 
			  printf("DEC Instruction"); 
  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.C = (temp1 == 0100000) ? 1:0;  
	
			  break;
		case ADC: 
			  printf("ADC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 + psw.C; 
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);} 

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = ((temp1 == 0077777) && (psw.C)) ? 1 : 0; 
			  psw.C = ((temp1 == 0177777) && (psw.C)) ? 1 : 0; 

			  break;

		case SBC: 
			  printf("SBC Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = temp1 - psw.C; 
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}

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
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
			  

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = (temp1 == 0100000) ? 1 : 0; 
			  psw.C = (result == 0) ? 0 : 1; 

			  break;
		case COM: 
			  printf("COM Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = ~temp1; 
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
			   
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
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = psw.N ^ psw.C;
			  break;
		case ROL: 
			  printf("ROL Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  temp2 = (temp1 << 1) | (psw.C) ;
			  psw.C = (temp1 & 0100000); 
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
			   
			   

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.V = psw.N ^ psw.C; 
			  break;
		case ASR: 
			  printf("ASR Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = (temp1 & 0100000) | (temp1 >> 1);
			  /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
		    	  
			   

  			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0;  
			  psw.C = (temp1 & 0000001); 
			  psw.V = psw.N ^ psw.C; 
			  break;
		case ASL: 
			  printf("ASL Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = (temp1 & 0000001) | (temp1 << 1); 
	  	          /* only for instruction that reuse destination */
			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
	
			  psw.Z = (result == 0) ? 1 : 0;
			  psw.N = (result < 0) ? 1 : 0; 
			  psw.C = (temp1 & 0100000);
			  psw.V = psw.N ^ psw.C; 
			
			  break;
		case SWAB:
			  printf("SWAB Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = ((temp1 && 0xff00) >> 8) | ((temp1 && 0x00ff) << 8);			  
			  /* only for instruction that reuse destination */

			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}



  			  psw.Z = ((result & 0x00ff) == 0 ) ? 1 : 0;
			  psw.N = ((result & 0x0080) >> 7) ? 1 : 0; 
			  psw.V = 0; 
			  psw.C = 0;
			  break;
		case SXT:
			  printf("SXT Instruction\n");
			  temp1 = reg_READ(trace,input_var.OOP.Mode,input_var.OOP.Register); 
			  result = (psw.N == 1) ? 0177777 : 0000000; 
			  /* only for instruction that reuse destination */

			  if(Mode != 0){write_mem(trace,DATA_WRITE,current_EA,result);}
			  else{reg_WRITE(trace,input_var.OOP.Mode, input_var.OOP.Register, result);}
 


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
	temp = Reg[PC];
	printf("Offset = %d\n",input_var.BRANCH.Offset); 
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

/* JUMP Instruction */ 
void func_jump(FILE *trace, instruction_set input_var){
	/* JUMP */
	/* JUMP TO ADDRESS - 0001AA */ 
	if(((input_var.fetched & 0177700) >> 6) == 00001){
		 #ifdef DEBUG
			printf("JUMP instruction\n"); 
		 #endif 
		  
		 Reg[PC] = (input_var.fetched & 0000077); 		
	}
	/* RTS */
	/* Return from Subroutine  - 00020R */ 
	else if(((input_var.fetched & 0177700) >> 6)== 000020){
		#ifdef DEBUG 
			printf("RTS Instruction\n");
		#endif
		unsigned short R = (input_var.fetched & 0000007); 
		Reg[PC] = Reg[R]; 
		R = read_mem(trace,DATA_READ,Reg[SP]);
		Reg[SP] = Reg[SP] + 2; 
	}
	/* JSR  */
	/* Jump to Subroutine - 004RAA */ 
	else if(((input_var.fetched & 0177000) >> 9) == 0004){
		#ifdef DEBUG 
			printf("RTS Instruction\n");
		#endif
		unsigned short temp = (input_var.fetched & 0000077);
		Reg[SP] = Reg[SP] - 2;
		unsigned short R = (input_var.fetched & 0000700) >> 6; 
		Reg[SP] = Reg[R]; 
		Reg[R] = Reg[PC]; 
		Reg[PC] = temp; 
	}
} 

/* PSW Instruction */
void func_psw(FILE *trace, instruction_set input_var){
	unsigned short octal = input_var.fetched; 
	/* Set Priority Level */ 
	if((octal >> 1) == 000023){
		#ifdef DEBUG 
			printf("Set Priority Level\n"); 
		#endif
	}
	/* CLC - Clear C */
	else if(octal == CLC){
		#ifdef DEBUG
			printf("Clear N"); 
		#endif
		psw.C = 0; 
	}
	/* CLV - Clear V */
	else if(octal == CLV){
		#ifdef DEBUG
			printf("Clear V");  
		#endif
		psw.V = 0;
	}
	/* CLZ - Clear Z */
	else if(octal == CLZ){
		#ifdef DEBUG
			printf("Clear Z");  
		#endif
		psw.Z = 0;
	} 
	/* CLN - Clear N */
	else if(octal == CLN){
		#ifdef DEBUG
			printf("Clear N"); 
		#endif
		psw.N = 0;
	} 
	/* SEC - Set C */
	else if(octal == SEC){
		#ifdef DEBUG
			printf("Set C"); 
		#endif
		psw.C = 1;
	} 
	/* SEV - Set V*/
	else if(octal == SEV){
		#ifdef DEBUG
			printf("Set V") 
		#endif
		psw.V = 1;
	} 
	/* SEZ - Set Z */
	else if(octal == CLZ){
		#ifdef DEBUG
			printf("Set Z"); 
		#endif
		psw.Z = 1;
	} 
	/* SEN - Set N */
	else if(octal == CLZ){
		#ifdef DEBUG
			printf("Set N"); 
		#endif
		psw.N = 1;
	} 
	/* CLC - Clear C */
	else if(octal == CLZ){
		#ifdef DEBUG
			printf("Clear Conditions Codes\n"); 
		#endif 
		psw.C = 0; psw.V = 0; psw.Z = 0; psw.N = 0; 
	} 
	/* SCC - Set Condition codes */
	else if(octal == SCC){
		#ifdef DEBUG
			printf("Set Condition Code\n");
		#endif
		psw.C = 1; psw.V = 1; psw.Z = 1; psw.N = 1; 
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
			#ifdef DEBUG
			printf("Mem Value - %o, Mem Index - %d\n",mem[mem_pointer], mem_pointer);
			#endif
			mem[mem_pointer+1] = (octal_value & 0177400) >> 8; 
			#ifdef DEBUG
			printf("Mem Value - %o, Mem Index - %d\n",mem[mem_pointer+1], mem_pointer+1); 
			printf("Final Value - %o\n", (mem[mem_pointer+1] << 8) | (mem[mem_pointer])); 
			#endif 
			mem_pointer = mem_pointer + 2; 
		}
	
 
	}
	fclose(fp); 
	return(0); 
}
	









 




