#include <stdio.h> 


int main(){

	signed short  x = 0x8080;
	signed short y = -x; 


	signed short output_x = ((y & 0xff00) >> 8) | ((y & 0x00ff) << 8); 
	printf("output %0x\n",output_x); 

	return (0);
}
