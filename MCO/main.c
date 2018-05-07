/*
@author:    Ijaz Ahmad

@descp:     This Program configures PA.8 Pin (MCO1)
            to output HSI clock as Test Clock.

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
    
  /* MCO1 --> PA8 */
  
  /*
    Enable clock to GPIOA
  */
  RCC->AHB1ENR |= 1U;
  
  /*********************
    PA.8 Configuration
  *********************/
  
  /* PA8 will be used for clock output (MCO1) and not 
    as GPIO so configure it for alternate funciton
  */
  
  GPIOA->MODER &= ~(1U << 16);
  GPIOA->MODER |= (1U << 17);
  
  /* Push Pull -> clock inlcludes both states */
  GPIOA->OTYPER &= ~(1U << 8);
  
  /* high speed */
  GPIOA->OSPEEDR |= (2U << 16);
  
  /* connect to alternate function AF0 (System Function)*/
  GPIOA->AFR[1] &= ~(0xFU);
  
  
  /********************
      MCO1 Settings 
  ********************/
  /*
    00: HSI clock selected    [WE WILL SELECT THIS ON]
    01: LSE oscillator selected
    10: HSE oscillator clock selected
    11: PLL clock selected
  */
  RCC->CFGR &= ~(3U << 21);
  
  /*
    Prescale by factor of 4 --> 4Mhz output on MCO1 (in range low quality analyzer)
  */
  RCC->CFGR &= ~(1U << 24);
  RCC->CFGR |=  (3U << 25);
  
  /*
    Now HSI clock is connected to MCO1
    MCO1 pin will now provide clock of 4Mhz.
  */
  
  while (1); //stop processor for test purposes
  
}

