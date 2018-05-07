/*
@author:    Ijaz Ahmad

@descp:     This Program configures the onchip
            RCC Clock Security System (CSS).

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdint.h>
#include "stm32f4xx.h"

#define       __setbit(___reg, ___bit)      ((___reg) |= (1U << (___bit)))
#define       __clearbit(___reg, ___bit)    ((___reg) &= (~(1U << (___bit))))
#define       __togglebit(___reg, ___bit)   ((___reg) ^= (1U << (___bit)))
#define       __getbit(___reg, ___bit)      (((___reg) & (1U << (___bit))) >> (___bit))


int main () {

  uint32_t regVal = 0;
  
  /*
    Turn ON Clock Security System
  */
  RCC->CR |=  (1U << 19);
  
  
  /*
    Enable HSE clock as the main system Clock
  */
  RCC->CR |= (1U << 16);
  
  /*
    wait until the external clock get stable
  */
  while (__getbit(RCC->CR, 17) != 1U);
  
  /*
    Switch to HSE Clock
    SW1, SW0
     0 ,  1
  */
  regVal = (RCC->CFGR & (~0x3U)); //read CFGR register and clear the lower two bits
  
  regVal |= 0x1;  // SW1, SW0 bits
  
  RCC->CFGR = regVal;

  while (1) {
    /* do other stuff here */
  }
  
}

/*
  Interrupt Handler for NMI
*/
void NMI_Handler (void) {

  /*
    Verify if code security system has generated as interrupt  
  */
  
  if ( RCC->CIR & (1U << 7)) {
    
    /*
      clear the interrupt
    */
    RCC->CIR |= (1U << 23);
    
    /*
     * Take appropriate action to reconfigure all the peripherals etc.
     * based on the new HIGH SPEED Internal RC OSCILLATOR-HSI (16Mhz);
     * AT THIS POINT SYSTEM IS RUNNING ON 16MHZ
     */
    
    /* place for reconfiguration */
  
  }
}

