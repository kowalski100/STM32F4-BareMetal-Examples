/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    This program uses STM32F4-Discovery Timer-2 
 *                  in UP/DOWN-COUNTER mode to generate constant 
 *                  time delay (0.5sec) /interrupt. Upon each interrupt,  
 *                  the on-board Blue-LED (PD#15) is toggled.
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

/* Functions Prototypes */
static void initLed(void);


/* SET UP_COUNTER TO 0 IF DOWN COUNTER IS REQUIRED */
#define     UP_COUNTER      1

int main () {
    
    /*
        For GPIO explanation go to LED Tutorial (GPIO-output)
    */
    initLed();
    
    /*****************************************
        Timers Configuration
     *****************************************/
    
    /* 
        From STM32F407 datasheet, Timer2 is clocked from
        APB1 bus (42Mhz max). In default configuration
        Timer-2 is receiving 16Mhz (HSI) bus clock.
    */
    
    /* Enable clock to Timer-2 on AHB-1 Bus */
    __setbit(RCC->APB1ENR, 0U);

    /*
        Divide the timer-2 input frequency (16Mhz)
        by a factor of 1000 (16,000,000/1,000 = 16,000 = 16Khz) 
    */
    TIM2->PSC = 1000;
    
    #if (UP_COUNTER)
     /* Up-Counter mode*/
     __clearbit(TIM2->CR1, 4U);
    #else
     /* Down Counter*/
     __clearbit(TIM2->CR1, 4U);
    #endif

    /*
        We want the time count to be 500msec (half a second).
        As the input frequency is 16khz so the total
        counts required for 500msec delay:
        
        total counts = 500msec * f
                     = (.5 sec) * 16,000
                     = 8,000
                     = 0x1F40
    */
    TIM2->ARR = 8000;

    /*
        Enable timer-2 Update interrupt to
        generate interrupt on counter overflow/underflow
    */
    __setbit(TIM2->DIER, 0U);


    /*
        Timer-2 interrupt is received on IRQ-6 on NVIC
        so enable IRQ-6 on NVIC for interrupt detection
    */
     NVIC_EnableIRQ(TIM2_IRQn);
     
     /* Start Timer-2 */
     __setbit(TIM2->CR1, 0U);
    
    for (;;) {}

}

/*********************************************************
    Timer-2 Interrupt Handler
**********************************************************/

void TIM2_IRQHandler (void) {
    
    /* clear timer interrupt */
    __clearbit(TIM2->SR, 0U);
    
    /* Toggle BLUE-LED PD#15 on timer interrupt*/
    __togglebit(GPIOD->ODR, 15);
}

static void initLed(void) {

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

