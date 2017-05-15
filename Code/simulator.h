/* Simulator Header File
   Author: Apurva Gandole, Chandan Muralidhar, Shadman Samin, Sourabh Balakrishna 

   Description: Includes all the Register defines, flag defines, type define, function prototypes 

*/ 
#include <stdio.h>

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
#define C 0
#define V 1
#define Z 2
#define N 3
#define T 4
#define I 5

/* Mnemonic Opcode */

/// -------------DOUBLE OPERAND-------------------- 
#define MOV 1		// Move: dest = src
#define MOVB 11		
#define CMP		// Compare : compute src - dest, set flags only
#define CMPB 12 	
#define BIT 3
#define BITB 13		// Bit test: compute dest & src, set flags only
#define BIC 3
#define BICB 14		//Bit clear: dest &= ~src 
#define BIS 5
#define BISB 15		// Bit set, a.k.a logic OR: dest |= src 
#define ADD 6		// ADD, dest += src 
#define SUB 16 		// Substract, dest -=  src 
///-------------------------------------------------

///--------------------------------------------------
#define MUL 70		// Multiply: (R,R+1) = R*SRC 
#define DIV 71		// Divide: Compute (R,R+1) / SRC, quotient in R, remainder R+1
#define ASH 72		// Arithmetic shift: R <<= src, shift amount maybe be -32...31
#define ASHC 73 	// Arithmetic shift combined: (R,R+1) <<= src, shift amount may be -32... 31
#define XOR 74 		// Exclusive OR: dest ^= reg (word only) 
#define SOB 77		// Subtract and branch: decrement register, if result non-zero, branch backward 0...63 words
//---------------------------------------------------




/* Structures - For Decode */
/* struct [structure tag] {
	member definition;
	...
	member definition; 
} [one or more structure variables] 
*/

//// Double-operand instructions /// 
struct double_operand{
	unsigned short B:1; 
	unsigned short Opcode:3;
	unsigned short Mode_1:3;
	unsigned short Source:3;
	unsigned short Mode_2:3;
	unsigned short Destination:3;
}double_operand;

struct single_operand{
	unsigned short B:1;
	unsigned short Remainder:4;            
	unsigned short Opcode:5; 
	unsigned short Mode:3;
	unsigned short Register:3;
}single_operand;

struct conditional{
	unsigned short x:1;
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
union instruction_set{
	struct double_operand DOP; 
	struct single_operand SOP;
	struct conditional COND; 
} instruction_set;

// Function prototypes
int open_file(char *arr);
void display();  
void write_mem(FILE*,unsigned short type, unsigned short address, unsigned short data);
unsigned short read_mem(FILE*, unsigned short type, unsigned short address);


















 








