/*
@author:    Ijaz Ahmad

@descp:     This Program configures PD.15 as Output
            and blinks on-board Blue - LED connected
            to PD.15.

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
    
    volatile unsigned int i = 0 ;
    
    // enable clock to GPIOD
    __setbit(RCC->AHB1ENR, 3);
    
    /*
        On STM32F4-Discovery
        BLUE-LED     -->   PD.15  
    */
    
    /*
        configure PD.15 as general purpose output
    */
    __setbit(GPIOD->MODER, 30);
    __clearbit(GPIOD->MODER, 31);
    
    /*
        GPIOs output type: Push-Pull
        so that it can be pulled high (logic-1) and pulled down (logic-0)
    */
    __clearbit(GPIOD->OTYPER, 15);
    
    /*
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOD->OSPEEDR, 30);
    __clearbit(GPIOD->OSPEEDR, 31);
    
    /*
    Initial Level: Logic High --> No Pull-up/down
    */
    __clearbit(GPIOD->PUPDR, 30);
    __clearbit(GPIOD->PUPDR, 31);
    
    
    while (1) {
       __togglebit(GPIOD->ODR, 15);
       for ( i = 0; i < 100000; ++i);
    }

}

