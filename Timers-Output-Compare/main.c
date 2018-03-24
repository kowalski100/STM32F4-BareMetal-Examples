/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    This code configures Timer-4 output
 *                  channels, connected to onboard GPIOS
 *                  in ouput compare mode. Each Channel
 *                  upon successful match, toggle one of
 *                  the onboard LED.
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
static void initLEDs(void);
static void initTimer(void);


/* SET UP_COUNTER TO 0 IF DOWN COUNTER IS REQUIRED */
#define     UP_COUNTER      1

int main () {
    
    /*
        For GPIO explanation go to LED Tutorial (GPIO-output)
    */
    initLEDs();
    
    /*
        init Timer-4 and Output Compare Channel
    */
    initTimer();
    
    for (;;) {}
}

static void initTimer(void) {
    
    /*****************************************
        Timers Configuration
     *****************************************/
    
    /* 
        From STM32F407 datasheet, Timer2 is clocked from
        APB1 bus (42Mhz max). In default configuration
        Timer-4 is receiving 16Mhz (HSI) bus clock.
    */
    
    /* Enable clock to Timer-4 on AHB-1 Bus */
    __setbit(RCC->APB1ENR, 2U);

    /*
        Divide the timer-4 input frequency (16Mhz)
        by a factor of 1000 (16,000,000/1,000 = 16,000 = 16Khz) 
    */
    TIM4->PSC = 1000;
    
    #if (UP_COUNTER)
     /* Up-Counter mode*/
     __clearbit(TIM4->CR1, 4U);
    #else
     /* Down Counter*/
     __clearbit(TIM4->CR1, 4U);
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
    TIM4->ARR = 8000;
     
    /* Start Timer-4 */
    __setbit(TIM4->CR1, 0U);


    /****************************************
          CONFIGURE TIMER CHANNEL
    ****************************************/

    /*
      Set all four channel to output mode (default mode)
    */
    __clearbit(TIM4->CCMR1, 0U);
    __clearbit(TIM4->CCMR1, 1U);

    __clearbit(TIM4->CCMR1, 8U);
    __clearbit(TIM4->CCMR1, 9U);

    __clearbit(TIM4->CCMR2, 0U);
    __clearbit(TIM4->CCMR2, 1U);

    __clearbit(TIM4->CCMR2, 8U);
    __clearbit(TIM4->CCMR2, 9U);

    /*
        Timer channel to toggle OCxREF signal upon 
        compare
    */
    __setbit(TIM4->CCMR1, 4U);
    __setbit(TIM4->CCMR1, 5U);

    __setbit(TIM4->CCMR1, 12U);
    __setbit(TIM4->CCMR1, 13U);

    __setbit(TIM4->CCMR2, 4U);
    __setbit(TIM4->CCMR2, 5U);

    __setbit(TIM4->CCMR2, 12U);
    __setbit(TIM4->CCMR2, 13U);


    /*
        Set Compare Trigger Points
    */
    TIM4->CCR1 = 2000U;
    TIM4->CCR2 = 4000U;
    TIM4->CCR3 = 6000U;
    TIM4->CCR4 = 8000U;
    
    /*
        Enable Output Compare Channel
    */
    __setbit(TIM4->CCER, 0U);
    __setbit(TIM4->CCER, 4U);
    __setbit(TIM4->CCER, 8U);
    __setbit(TIM4->CCER, 12U);
    
}

static void initLEDs(void) {

    // enable clock to GPIOD
    __setbit(RCC->AHB1ENR, 3);
        
    /*
        configure PD.12,13,14,15 as alternate function pin
    */
    __clearbit(GPIOD->MODER, 24U);
    __setbit(GPIOD->MODER, 25U);

    __clearbit(GPIOD->MODER, 26U);
    __setbit(GPIOD->MODER, 27U);

    __clearbit(GPIOD->MODER, 28U);
    __setbit(GPIOD->MODER, 29U);

    __clearbit(GPIOD->MODER, 30U);
    __setbit(GPIOD->MODER, 31U);
    
    /*
        Connect GPIOD pins to Alternate function
        Peripheral. In this case we want to connect
        GPIOD Pins 12-15 to be connected to timer-4
        output compare channels. In order to do so,
        the GPIOD pins should implment AF2 (0x2).
        For more details on GPIOs, Visit the tutorial
        accompanied website.
    */
    GPIOD->AFR[1] = 0x0U;
    
    __setbit(GPIOD->AFR[1], 17U);
    __setbit(GPIOD->AFR[1], 21U);
    __setbit(GPIOD->AFR[1], 25U);
    __setbit(GPIOD->AFR[1], 29U);
    
}

