/*
@author:    Ijaz Ahmad

@descp:     This Program configures STM32f4-Discover
            on-board USER button and blinks on-board 
            Blue - LED connected to PD.15 when button 
            is pressed.

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


/*************************************************************
            Local Functions Prototypes
 *************************************************************/
static void configureUserBtn(void);
static void configureLed(void);

int main () {
    
    /* Configure Blue LED PD.15 */
    configureLed();
    
    /* Configure User Button PA.0 */
    configureUserBtn();
    
    while (1) {
        
        if (__getbit(GPIOA->IDR, 0)) {
            __setbit(GPIOD->ODR, 15); 
        }else {
            __clearbit(GPIOD->ODR, 15); 
        }

    }
    
}


static void configureLed(void) {

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
    
}

static void configureUserBtn(void) {
    
    // enable clock to GPIOA
    __setbit(RCC->AHB1ENR, 0);
    
    /*
        On STM32F4-Discovery
        User Btn   -->   PA.0  
    */
    
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

