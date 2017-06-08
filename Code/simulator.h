/* Simulator Header File
   Author: Apurva Gandole, Chandan Muralidhar, Shadman Samin, Sourabh Balakrishna 

   Description: Includes all the Register defines, flag defines, type define, function prototypes 

*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>

// DEFINES
#define byte_length 4 
#define KB 1024
#define MB (KB*KB) 
#define MEM (64*KB) 

#define DATA_READ 0 
#define DATA_WRITE 1 
#define INSTRUCTION_FETCH 2 

/* Registers */ 
#define R0 0
#define	R1 1
#define	R2 2
#define R3 3
#define R4 4 
#define R5 5
#define SP 6	
#define PC 7

/* Mnemonic Opcode */
/// ------------ ZERO OPERAND OPCODE --------------
#define HALT 0000000
#define NOP  0000240

/// -------------DOUBLE OPERAND-------------------- 
#define MOV 01		// Move: dest = src	
#define CMP 02		// Compare : compute src - dest, set flags only	
#define BIT 03 		// Bit test: compute dest & src, set flags only
#define BIC 04		//Bit clear: dest &= ~src 
#define BIS 05		// Bit set, a.k.a logic OR: dest |= src 
#define ADDSUB 06		// ADD, dest += src 
 	// Substract, dest -=  src 
///-------------------------------------------------

///--------------------------------------------------
#define MUL 070		// Multiply: (R,R+1) = R*SRC 
#define DIV 071		// Divide: Compute (R,R+1) / SRC, quotient in R, remainder R+1
#define ASH 072		// Arithmetic shift: R <<= src, shift amount maybe be -32...31
#define ASHC 073 	// Arithmetic shift combined: (R,R+1) <<= src, shift amount may be -32... 31
#define XOR 074 		// Exclusive OR: dest ^= reg (word only) 
#define SOB 077		// Subtract and branch: decrement register, if result non-zero, branch backward 0...63 words
//---------------------------------------------------


//=========== SINGLE OPERAND ===================
#define CLR 00050	// Clear 
#define INC 00052   	// Increment
#define DEC 00053	// Decrement
#define ADC 00055	// Add carry
#define SBC 00056	// Substract carry
#define TST 00057	// Test 
#define NEG 00054	// Negate
#define COM 00051	// Complement
#define ROR 00060	// Rotate right
#define ROL 00061	// Rotate left
#define ASR 00062	// arithmetic shift right
#define ASL 00063	// arithmetic shift left
#define SWAB 00003	// Swap bytes
#define SXT 00067	// sign extend
#define JMP 00001	// Jump


// Effective address calculation define //
#define REG		00
#define R_DEF   01
#define AUTOINCR     02
#define AUTOINC_DEF  03
#define AUTODECR     04
#define AUTODECR_DEF 05
#define INDEX	     06
#define INDEX_DEF    07

#define IMD	02
#define ABSDR	03
#define RLDIR	06
#define RLINDIR	07


// Branch Define
#define BR		01
#define BNE     02
#define BEQ     03
#define BPL     00
#define BMI     01
#define BVC     04
#define BVS     05
#define BHIS    06
#define BCC     06
#define BLO     07
#define BCS     07
#define BGE     04
#define BLT     05
#define BGT     06
#define BLE     07
#define BHI     02
#define BLOS    03


// PSW Define
#define CLC 0000241
#define CLV 0000242
#define CLZ 0000244
#define CLN 0000250
#define SEC 0000261
#define SEV 0000262
#define SEZ 0000264
#define SEN 0000270
#define CCC 0000257
#define SCC 0000277




/* Structures - For Decode */
/* struct [structure tag] {
	member definition;
	...
	member definition; 
} [one or more structure variables] 
*/

//// Double-operand instructions /// 
struct double_operand{
	unsigned short Destination:3;
	unsigned short Mode_D:3;
	unsigned short Source:3;
	unsigned short Mode_S:3;
	unsigned short Opcode:3;
	unsigned short B:1; 
}double_operand;

struct single_operand{            
	unsigned short Register:3;
	unsigned short Mode:3;
	unsigned short Opcode:10; 
}single_operand;

struct onehalf_operand{
	unsigned short Source:3;
	unsigned short Mode:3;
	unsigned short Register:3;
	unsigned short Opcode:3;
	unsigned short Remainder:4; 
} onehalf_operand; 

struct conditional{
	signed short Offset:8;
	unsigned short Opcode:3;
	unsigned short Remainder:4;
	unsigned short x:1; 
}conditional;

/* PSW Structuree */ 
struct psw{
	unsigned short N:1;	// Negatives
	unsigned short Z:1;	// Zeros
	unsigned short V:1;	// Overflow
	unsigned short C:1;     // Carry
	unsigned short T:1;	// Trap
	unsigned short I:11;    // Interrupt Priority Level
}psw;  

 

/* Unions */ 
/* union [union tag]{
	member definition;
	...
	member definition;
}[one or more union variables]; 
*/ 
typedef union instruction_set{
	unsigned short fetched; 
	struct single_operand OOP;
	struct onehalf_operand OHOP;
	struct double_operand TOP;
	struct conditional BRANCH;  
} instruction_set;

// Variable Declaration
 

// Function prototypes
int open_file(char *arr);
void display();  
void print_REG(); 
void write_mem(FILE* trace,unsigned short type, unsigned short address, unsigned short data);
unsigned short read_mem(FILE* trace, unsigned short type, unsigned short address);
unsigned short Effective_address(FILE* trace, unsigned short mode, unsigned short source); 

/* Operand Function Prototypes */ 
void func_doubleoperand(FILE* trace,instruction_set input_var); 
void func_singleoperand(FILE* trace,instruction_set input_var);
void func_onehalfoperand(FILE* trace,instruction_set input_var);  
void func_conditionalbranch(FILE* trace,instruction_set input_var); 
void func_jump(FILE* trace,instruction_set input_var); 
void func_psw(FILE* trace,instruction_set input_var); 
void func_otherinstruction(FILE* trace,instruction_set input_var);

/* Register Read and Write Functions */ 
signed short reg_READ(FILE* trace, unsigned short mode, unsigned short source);
void reg_WRITE(FILE* trace, unsigned short mode, unsigned short destination,signed short data);













 








