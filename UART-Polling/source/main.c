/*
@author:    Ahmad
@descp:     This Program configures USART3 for asynchronous
            communication with 9600 baudrate.
            PB10 ----> Tx
            PB11 ----> Rx
            
@Method:    Polling

@warrenty:  void
*/
/**************************************************************
REQUIREMENTS:
    1. 9600 baud rate
    2. 8-bits data bits
    3. 1 start/stop bits
    4. No Parity
    
APPLICATION: Application should regularly Poll to 
             see if there any serial data pending
***************************************************************/

/*  
    Web Link: 
*/

/*************************************************************
STEPS:
    1. Configure GPIOs to have alternate function
    2. Configure UART
    3. Poll for Data Rx/Tx
*************************************************************/
#include <stdint.h>
#include "stm32f4xx.h"

#define       __setbit(___reg, ___bit)      ((___reg) |= (1U << (___bit)))
#define       __clearbit(___reg, ___bit)    ((___reg) &= (~(1U << (___bit))))
#define       __togglebit(___reg, ___bit)   ((___reg) ^= (1U << (___bit)))
#define       __getbit(___reg, ___bit)      (((___reg) & (1U << (___bit))) >> (___bit))



void initUSART(void);
void put_char(int ch); 
void transmitString( char * buffer );

int main () {

    char rxCh;
 
    initUSART();
  
    while (1) {
      
      /************************

          Polling for Data
           
      ************************/
        
      /*
          see if there is any data received
      */
      if  (__getbit(USART3->SR,5) == 1U) {
          
        /* receive character */
        rxCh = USART3->DR;  
        
        /* loop back received character */
        put_char(rxCh);
               
      }
   }
}

void initUSART(void) {

    /*
        USART3 Pins:
            PB10 ----> Tx
            PB11 ----> Rx
    */
    
    /* 
        Before we can use Port.B (PB) clock must be enable to it. 
    */    
    __setbit(RCC->AHB1ENR, 1);
    
    /******************************************************************
     *
     *    USART Tx, Rx pins are transition sensitive so set the initial
     *    GPIO Pins (Tx,Rx) state.
     *
     ******************************************************************/
    
    /* 
        Pins Output Type: Push Pull
        So that its state can be pulled up (logic-1), pulled down (logic-0).
    */
    __clearbit(GPIOB->OTYPER, 10);
    __clearbit(GPIOB->OTYPER, 11);
    
    /*
        Set Pins speed to medium.
    */
    /* ------ for PB10 ------ */
    __setbit(GPIOB->OSPEEDR, 20);
    __clearbit(GPIOB->OSPEEDR, 21);
    
    /* ------ for PB11 ------ */
    __setbit(GPIOB->OSPEEDR, 22);
    __clearbit(GPIOB->OSPEEDR, 23);
    
    /*
        Initial Pin State: High
        i.e. idle state is high
    */
    /* ------ for PB10 ------ */
    __setbit(GPIOB->PUPDR, 20);
    __clearbit(GPIOB->PUPDR, 21);
 
    /* ------ for PB11 ------ */
    __setbit(GPIOB->PUPDR, 22);
    __clearbit(GPIOB->PUPDR, 23);
    
    
    /******************************************************************
     *
     *    Now GPIO module and Pins states are set according to UART, 
     *  next thing is to connect GPIO pins to UART as an alternate function
     *      see the GPIO alternate function tutorial for more info.
     *
     ******************************************************************/
    /*
        Configure GPIO Pins as alternate function
    */
    /* ------ for PB10 ------ */
    __clearbit(GPIOB->MODER, 20);
    __setbit(GPIOB->MODER, 21);
 
    /* ------ for PB11 ------ */
    __clearbit(GPIOB->MODER, 22);
    __setbit(GPIOB->MODER, 23);

    /*
        Connect PB10 Pin to USART. AF7
    */
    __setbit(GPIOB->AFR[1], 8);
    __setbit(GPIOB->AFR[1], 9);
    __setbit(GPIOB->AFR[1], 10);    
    __clearbit(GPIOB->AFR[1], 11);

    /*
        Connect PB10 Pin to USART. AF7
    */
    __setbit(GPIOB->AFR[1], 12);
    __setbit(GPIOB->AFR[1], 13);
    __setbit(GPIOB->AFR[1], 14);
    __clearbit(GPIOB->AFR[1], 15);


    /******************************************************************
     *
     *      Now GPIO state is set and connected to USART Module
     *         The final step is to Configure USART Moduele
     *
     ******************************************************************/
    /*
        USART3 module is connected on APB1, before we can use USART3,
                        clock must be enabled to it.
    */
    __setbit(RCC->APB1ENR, 18);
    
    /*
        By default the clock to USART3 is High Speed Internal clock 
        HSI = 16Mhz
        
        Baudrate =  Fclk/ 8 x ( 2 - Over8 ) x USARTDIV
        
        OR

        USARTDIV =  Fclk/ 8 x ( 2 - Over8 ) x Baudrate

        Fclk = 16000000Hz
        
        Over8 = 0
        
        Required BR = 9600
        
        USARTDIV =  16000000 / 8 x ( 2 - 0 ) x 9600
        
        USARTDIV =  104.1666
        
        Fraction Part:  16 x 0.1666 = 2.656 ~= 3
        Mantissa:       104 = 0x68
        
        USART_BRR = Mantissa << 4 | Fraction
        
        USART_BRR = 0x683
        
        USART_BRR = 11010000011
    */
    USART3->BRR = 0x683;
    
    /*
        Disable Hardware Flow Control
    */
    __clearbit(USART3->CR2 , 8);
    __clearbit(USART3->CR2 , 9);
    
    /*
        1-start bit, 8-bit data
    */
    __clearbit(USART3->CR1 , 12);
    
    /*
        1-stop bit
    */
    __clearbit(USART3->CR2 , 12);
    __clearbit(USART3->CR2 , 13);
    
    /*
        No Parity bit
    */
    __clearbit(USART3->CR1 , 10);
    
    /*
        Enable both data Transmission and Reception
    */
   __setbit(USART3->CR1 , 2);
   __setbit(USART3->CR1 , 3);


    /*
        Finally powerup USART3 module
    */
   __setbit(USART3->CR1 , 13);  

}

void transmitString( char * buffer ) {

    volatile int i = 0;
    
    while ( buffer[i] != '\0' ) {
      put_char(buffer[i]);
      ++i;
    }
}

void put_char(int ch) {
  
  USART3->DR = ch;

  /*
    Wait until the data is transmitted
  */
  while ( __getbit(USART3->SR, 7) != 1);
  
}

