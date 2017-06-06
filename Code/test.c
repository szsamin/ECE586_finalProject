#include <stdio.h> 


int main(){

	signed short  x = -10;
	signed int  y = x+20; 


	signed short output_x = ((y & 0xff00) >> 8) | ((y & 0x00ff) << 8); 
	printf("output %d\n",y); 

	return (0);
}
