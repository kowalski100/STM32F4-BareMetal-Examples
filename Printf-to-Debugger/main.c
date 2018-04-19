/*
@author:    Ijaz Ahmad

@descp:     This Program redirects printf string to
			STLINK-v2 (via ARM ITM cell). i.e. printf
			messages will can be received from same
			usb cable connected to STM32f4-Discovery
			board that programms the onboard micro-
			controller- no extra hardware is required.

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdio.h>

int main () {
	volatile int i = 0;
	
	while (1) {
		
		/*
			This string will be redirected to STLINK on STM32f4-Discovery
		*/
		printf("String redirection to STlink Debugger :-)\n");
		
		for (i = 0; i < 5000; i++);
	}
}

