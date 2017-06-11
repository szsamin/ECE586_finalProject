#include <stdio.h> 


int main(){

	signed short  x = 0177777;
	signed int  y = 027777777777; 

	signed short mask = (y & 0100000) >> 15;
	signed short mask1 = (x & 0100000) >> 15; 

	printf("Mask - %o, Mask1 - %o\n",mask,mask1); 


	return (0);
}
