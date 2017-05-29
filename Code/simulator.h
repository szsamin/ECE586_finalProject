/* Simulator Header File
   Author: Apurva Gandole, Chandan Muralidhar, Shadman Samin, Sourabh Balakrishna 

   Description: Includes all the Register defines, flag defines, type define, function prototypes 

*/ 
#include "/usr/include/stdio.h"
#include "/usr/include/string.h"

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
 
/* Flags  */ 
#define C 1
#define V 2
#define Z 4
#define N 8
#define T 16
#define I 32

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

// Effective address calculation define //
#define REG		00
#define R_DEF   01
#define AUTOINCR     02
#define AUTOINC_DEF  03
#define AUTODECR     04
#define AUTODECR_DEF 05
#define INDEX	     06
#define INDEX_DEF    07

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






/* Structures - For Decode */
/* struct [structure tag] {
	member definition;
	...
	member definition; 
} [one or more structure variables] 
*/

/*
/// Zero Operand Instructions // 
typedef struct zero_operand{
	unsigned short opcode; 
} zero_operand; 
 
// One Operand Instructions // 
typedef struct one_operand{ 
	unsigned short opcode:10;
	unsigned short addr:6;
} one_operand; 


// one-and-half-operand instructions // 
typedef struct one_half_operand{
	unsigned short opcode:7;
	unsigned short reg:3;
	unsigned short addr:6; 
}one_half_operand; 



// two-operand instructions // 
typedef struct two_operand{
	unsigned short opcode:4;
	unsigned short src:6;
	unsigned short dst:6;
}two_operand; 



// branch instruction // 
typedef struct branch{
	unsigned short opcode:8;
	unsigned short offset:8;
}branch; 

*/

//// Double-operand instructions /// 
struct double_operand{
	unsigned short B:1; 
	unsigned short Opcode:3;
	unsigned short Mode_S:3;
	unsigned short Source:3;
	unsigned short Mode_D:3;
	unsigned short Destination:3;
}double_operand;

struct single_operand{
	unsigned short B:1;
	unsigned short Remainder:4;            
	unsigned short Opcode:5; 
	unsigned short Mode:3;
	unsigned short Register:3;
}single_operand;

struct onehalf_operand{
	unsigned short Remainder:4;
	unsigned short Opcode:3;
	unsigned short Register:3;
	unsigned short Mode:3;
	unsigned short Source:3;
} onehalf_operand; 

struct conditional{
	unsigned short x:1;
	unsigned short Remainder:4; 
	unsigned short Opcode:3;
	unsigned short Offset:8;
}conditional; 

 

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

// Function prototypes
int open_file(char *arr);
void display();  
void write_mem(FILE*,unsigned short type, unsigned short address, unsigned short data);
unsigned short read_mem(FILE*, unsigned short type, unsigned short address);
unsigned short Effective_address(FILE*, unsigned short mode, unsigned short source); 

/* Operand Function Prototypes */ 
void func_doubleoperand(FILE *,instruction_set input_var); 
void func_singleoperand(FILE *,instruction_set input_var);
void func_onehalfoperand(FILE *,instruction_set input_var);  
void func_conditionalbranch(FILE *,instruction_set input_var); 
void jump(FILE *,instruction_set input_var); 
void psw(FILE *,instruction_set input_var); 
void func_otherinstruction(FILE *,instruction_set input_var);

/* Register Read and Write Functions */ 
unsigned short reg_READ(FILE *trace, unsigned short mode, unsigned short source);
void reg_WRITE(FILE *trace, unsigned short mode, unsigned short destination,unsigned short data);













 








