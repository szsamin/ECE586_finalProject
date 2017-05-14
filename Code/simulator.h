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


















 








