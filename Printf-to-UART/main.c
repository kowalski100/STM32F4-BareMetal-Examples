/*
@author:    Ijaz Ahmad

@descp:     This Program configures USART3 and redirect
			printf strings to USART3 Tx PB.10

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stm32f4xx.h>
#include <stdio.h>

static void init_usart3 (void);
int stdout_putchar (int);

int main () {
	
	volatile int x = 0;
	
	/*
		configure DESIRED USART to which printf string will 
		be redirected. 
	*/
	init_usart3();
	
	for (;;) {
		
		/*
			Transmit string PB.10
		*/
		printf ("Test String redirection to UART\n");

		for (x = 0; x < 5000; x++);
	}
}

int stdout_putchar (int ch) {
	
	USART3->DR = ch;
	
	/* wait until the transmission completes */
	while ((USART3->SR & (1U << 6)) != (1U << 6));
	
	return 1;
}

static void init_usart3 (void) {

	/*
		enable clock to GPIOB
	*/
	RCC->AHB1ENR |= (1u << 1);
	
	/*
		enable clock to USART3
	*/
	RCC->APB1ENR |= (1U << 18);
	
	/*
		1-stop bit
	*/
	USART3->CR2	&=  ~(1u << 12);
	USART3->CR2	&=  ~(1u << 13);
	
	/*
		9600 baudrate at 16Mhz
		B.R = f / 8 x (2 - OVER8) x USARTDIV
		
		B.R = 9600
		f = 16000000
		OVER8 = 0
		
		==> USARTDIV 	= 104
		
		fraction = 0
		mantissa = 104
	*/
	
	USART3->BRR = (104U << 4);
	
	/*
		enabble Transmission
	*/
	USART3->CR1 |= (1U <<3);
	
	/*
		disable parity
	*/
	USART3->CR1 &= ~(1U << 10);
	
	/*
		1-start bit, 8-bits data, n-stop bits
	*/
	USART3->CR1 &= ~ (1U << 12);
	
	
	/********************************
		
	 ENABLE GPIOB Pin.10 as USART Tx
			
	*********************************/

	/*
		setting alternate function for GPIOB.10
	*/
	GPIOB->MODER |= (1U << 21);
	
	/* 
		medium speed 
	*/
	GPIOB->OSPEEDR |= (1U << 20);
	
	/*
		pull up
	*/
	GPIOB->PUPDR  |= (1U << 20);
	
	/*
		alternate function - 7 (USART-3 Tx)
	*/
	GPIOB->AFR[1] |=  (0x7U << 8);
	
	/*
		finally enable USART3
	*/
	USART3->CR1 |= (1U << 13);
	
}

