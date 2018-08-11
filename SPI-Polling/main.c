/*
@author:    Ijaz Ahmad

@descp:     In this tutorial we will send 0x55 from 
            SPI-1 (master) to SPI-2. Upon successful 
            reception, PD.15 (on board BLUE LED) is
            toggled.

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

void configLED(void);
void configUserBtn (void);
void configureSPIPins(void);
void configureSPIBus(void);

int main () {

  volatile unsigned int i = 0;
  
    configLED();
    configUserBtn();
    configureSPIPins();
    configureSPIBus();
  
    while (1) {
      
      /*
        check if button is pressed
      */
      if (GPIOA->IDR &0x1) {

        /*
          compensate for button debounce problem
        */
        for (i = 0; i < 40000; i++);
        
        /* read after debounce compensation */
        if (GPIOA->IDR &0x1) {

          /* send data from SPI-1 */
          SPI1->DR = 0x55;
        
          /* wait unit data is transmitted */
          while (!(SPI1->SR & 2U));
        
          /*
            prevent multiple transmission on single button press
          */
          while (GPIOA->IDR & 0x1U);
        }
      }
      
      /*
        check if any data is received
      */
      if (SPI2->SR & 1U){
        
        /*
            receive SPI-2 data and check data validity
        */
        if (SPI2->DR == 0x55) {        
          __togglebit(GPIOD->ODR, 15);
        }
      }
      
    }
}

void configureSPIBus(void) {

    /**************************************  
           SPI-1 - Configuration          
          
            -> As a bus Master
    ***************************************/
  /*
    enable clock to SPI-1
  */
  __setbit(RCC->APB2ENR,12);
  
  /*
    SPI-1 as bus master
  */
  __setbit(SPI1->CR1, 2);
  
  /*
      CPOL = HIGH
  */
  __setbit(SPI1->CR1, 1);

  /*
      CPHASE = 1-Edge
  */
  __clearbit(SPI1->CR1, 0);
  
  /*
      pre-scale by 16 ~512kbps
  */
  SPI1->CR1 |= (3U << 3);
  __clearbit(SPI1->CR1, 5);

  /* MSB first */
  __clearbit(SPI1->CR1, 7);
  
  /* Software Slave Management */
  __setbit(SPI1->CR1, 9);
  __setbit(SPI1->CR1, 8);
  
  /*
    full-duplex communication
  */
  __clearbit(SPI1->CR1, 10);
  
  /*
     8-bit data frame/packet
  */
  __clearbit(SPI1->CR1, 11);
  
  /*
     No CRC
  */
  __clearbit(SPI1->CR1, 13);
  
  /*
     2-lines uni-directional mode
  */
  __clearbit(SPI1->CR1, 15);
  
  /*
    enable SPI-1 Unit
  */
  __setbit(SPI1->CR1, 6);
  


    /**************************************  
            SPI-2 - Configuration          
          
            -> As a bus Slave
    ***************************************/
    /*
      enable clock to SPI-2
    */
  __setbit(RCC->APB1ENR,14);
  
  /*
    SPI-2 as slave device
  */
  __clearbit(SPI2->CR1, 2);
  
  /*
      CPOL = HIGH
  */
  __setbit(SPI2->CR1, 1);

  /*
      CPHASE = 1-Edge
  */
  __clearbit(SPI2->CR1, 0);
  

  /* MSB first */
  __clearbit(SPI2->CR1, 7);
  
  /* Software Slave Management */
  __setbit(SPI2->CR1, 9);

  
  /*
    full-duplex communication
  */
  __clearbit(SPI2->CR1, 10);
  
  /*
     8-bit data frame/packet
  */
  __clearbit(SPI2->CR1, 11);
  
  /*
     No CRC
  */
  __clearbit(SPI2->CR1, 13);
  
  /*
     2-lines uni-directional mode
  */
  __clearbit(SPI2->CR1, 15);
  
  /*
    enable SPI-2 Unit
  */
  __setbit(SPI2->CR1, 6);

}

void configureSPIPins(void) {
  
    /**************************************  
                SPI-1 - Pins          
          Pins:
            PA.5  ->  SCK
            PA.6  ->  MISO
            PA.7  ->  MOSI
    ***************************************/

  /*
    enabled clock to to PA
  */
  __setbit(RCC->AHB1ENR, 0);
  
  /*
     PA.5-7 Configuratoin
     -> See GPIOs tutorial for more info
  */
  
  /*
    As an alternate function
  */
  GPIOA->MODER |=  (0xA8U <<8);

  /*
    output push-pull
  */
  GPIOA->OTYPER &= (~(0x7U << 5));
  
  /*
    Speed: very high
  */
  
  GPIOA->OSPEEDR |=  (0xFCU <<8);  
  
  /*
    connect PA.5-7 to SPI pins for alternate function
  */
  
  GPIOA->AFR[0] |= (0x555U << 20);
  


    /**************************************  
                SPI-2 - Pins          
          Pins:
            PB.10   ->  SCK
            PC.2    ->  MISO
            PC.3    ->  MOSI
    ***************************************/
  
  /*
     PB.10 Configuratoin
     -> See GPIOs tutorial for more info
  */

  /*
    enabled clock to to PB
  */
  __setbit(RCC->AHB1ENR, 1);

  /*
    As an alternate function
  */
  __clearbit(GPIOB->MODER, 20);
  __setbit(GPIOB->MODER, 21);


  /*
    output push-pull
  */
  GPIOB->OTYPER &= (~(0x1U << 10));
  
  /*
    Speed: very high
  */
  
  GPIOB->OSPEEDR |=  (0x3U <<20);  
  
  /*
    connect PA.5-7 to SPI pins for alternate function
  */
  
  GPIOB->AFR[1] |= (0x5U << 8);



  /*
     PC.2-3 Configuratoin
     -> See GPIOs tutorial for more info
  */

  /*
    enabled clock to to PC
  */
  __setbit(RCC->AHB1ENR, 2);

  /*
    As an alternate function
  */
  GPIOC->MODER |=  (0xAU << 4);

  /*
    output push-pull
  */
  GPIOC->OTYPER &= (~(0x3U << 2));
  
  /*
    Speed: very high
  */
  
  GPIOC->OSPEEDR |=  (0xFU << 4);  
  
  /*
    connect PC.2,3 to SPI pins for alternate function
  */
  
  GPIOC->AFR[0] |= (0x55U << 8);

}

void configLED(void) {


    /******************************************
  
            LED is connected to PD.15
  
    ****************************************/
    
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

void configUserBtn (void) {

   /******************************************
  
            User Btn is connected to PA.0
  
    ****************************************/

    
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
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOA->OSPEEDR, 0);
    __clearbit(GPIOA->OSPEEDR, 1);
    
    /*
      Initial Level: Pull Down --> Logic-0
    */
    __clearbit(GPIOA->PUPDR, 0);
    __setbit(GPIOA->PUPDR, 1);
    
}



