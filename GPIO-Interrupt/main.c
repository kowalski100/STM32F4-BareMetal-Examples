/*
@author:    Ijaz Ahmad

@descp:     The program configures PA.0 to generate interrupt
            on rising edge of input signal. Upon receiving 
            interrupt, PD.15 (BLUE-LED) is toggled.
            The signal to PA.0 is given from PD.14, which is
            also connected to on-board RED-LED.

@warrenty:  void

@Board:     STM32F4-Discovery
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

static void configureLEDs (void);
static void configInputPin (void);

int main () {
    
    volatile unsigned int i = 0 ;
    
    configureLEDs();
    configInputPin();
    
    /*
        Configure EXTI.0 for PA.0
    */
    
    /*
        PA.0 is connected on EXTI0 which in turn 
        connected to IRQ No.6
    */
    
    /*
        enable clock to syscfg for pin selection.
    */
    __setbit(RCC->APB2ENR, 14);
    
    /*
        connect PA.0 to EXTI.0
        EXTICR[3-0] = 0b0000
    */
    __clearbit(SYSCFG->EXTICR[0],0);
    __clearbit(SYSCFG->EXTICR[0],1);
    __clearbit(SYSCFG->EXTICR[0],2);
    __clearbit(SYSCFG->EXTICR[0],3);
    
   /*
        Enable Interrupt on EXTI0 line
   */ 
   __setbit(EXTI->IMR, 0);
   
   
   /*
       trigger interrupt on rising edge
   */
   __setbit(EXTI->RTSR, 0);
   
   /*
       Now EXTI has been configured, finally
       enable IRQn.6 to accept interrupt.
   */
   
   NVIC_EnableIRQ(EXTI0_IRQn);
    
    
    while (1) {
       __togglebit(GPIOD->ODR, 14);
       for ( i = 0; i < 1000000; ++i);
    }

}

static void configureLEDs (void) {

   // enable clock to GPIOD
    __setbit(RCC->AHB1ENR, 3);
    
    /*
        On STM32F4-Discovery
        BLUE-LED     -->   PD.15  
        BLUE-LED     -->   PD.14
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
        configure PD.14 as general purpose output
    */
    __setbit(GPIOD->MODER, 28);
    __clearbit(GPIOD->MODER, 29);
    
    /*
        GPIOs output type: Push-Pull
        so that it can be pulled high (logic-1) and pulled down (logic-0)
    */
    __clearbit(GPIOD->OTYPER, 14);
    
    /*
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOD->OSPEEDR, 28);
    __clearbit(GPIOD->OSPEEDR, 29);
    
}

static void configInputPin (void) {
    
     // enable clock to GPIOA
    __setbit(RCC->AHB1ENR, 0);
    
    
    /*
        configure PA.0 as digital input
    */
    __clearbit(GPIOA->MODER, 0);
    __clearbit(GPIOA->MODER, 1);
    
    /*
        GPIOs output type: Push-Pull
        so that it can be pulled high (logic-1) and pulled down (logic-0)
    */
    __clearbit(GPIOA->OTYPER, 0);
    
    /*
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOA->OSPEEDR, 0);
    __clearbit(GPIOA->OSPEEDR, 1);
    
    /*
    Initial Level: Logic High --> No Pull-up/down
    */
    __clearbit(GPIOA->PUPDR, 0);
    __setbit(GPIOA->PUPDR, 1);
    
}


void EXTI0_IRQHandler (void) {
    
    /*
        Clear the pending interrupt
    */
    __setbit (EXTI->PR, 0);
    
    /* 
        toggle the led to confirm that interrupt
        has called the ISR.
    */
    __togglebit(GPIOD->ODR, 15);
}

