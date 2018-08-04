/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    Selects and configures various clock sources of STM32F4-Discovery  
 *
 */

/*
 * @WebLink: 
 */

#include "stm32f4xx.h"
#include <stdint.h>

void SysClock_configHSI(void);
void SysClock_configHSE(void);
void SysClock_configPLL(void);

int main () {

  /*
    NOTE: uncomment the one required 
  */
  
//  SysClock_configHSI();  //Configure Internal 16Mhz oscillator as System Clock
//  SysClock_configHSE();  //Configure external 8Mhz clock as System Clock
  SysClock_configPLL();  //Configure PLL as System Clock (feed from internal 16Mhz oscillator)
  
  __ASM {    
    BKPT #0x3    
  }
  
}

/*
  select HSI as main system clock
*/
void SysClock_configHSI(void) {
  
  /*
     Turn ON HSI Clock [Default selected]
  */
  RCC->CR |= (1U << 0);

  /*
     wait until HSI gets stable
  */
  while (!(RCC->CR & (1U << 1)));

  /*
     switch clock to HSI
  */
  RCC->CFGR  &= (~0x3U);
  
}

/*
  select HSE as main system clock
*/
void SysClock_configHSE(void) {

  uint32_t regVal = 0;
  
  /*
     Turn ON HSE Clock [Default selected]
  */
  RCC->CR |= (1U << 16);

  /*
     wait until HSE gets stable
  */
  while (!(RCC->CR & (1U << 17)));

  /*
     switch clock to HSE
  */
  regVal = RCC->CFGR & (~(0x3U));
  regVal |= 0x1U;

  RCC->CFGR = regVal;
  
}

/*
  select PLL as main system clock
  SoC runs at its full capacity of 168Mhz
  PLL is feed from INTERNAL 16Mhz Oscillator
*/
void SysClock_configPLL(void) {

  uint32_t regVal = 0;
  
  /*
    disable PLL 
  */
  RCC->CR &= ~(1U << 24);

#define   PLLN    168U
#define   PLLM    8U
#define   PLLP    0U  /* 00: PLLP = 2 */
#define   PLLQ    7U
  
  /*
    configure PLL
    PLLN = 168
    PLLM = 8
    PLLP = 2
   
    f(vco) = f(input) * PLLN / PLLM
    f(vco) = 16Mhz * 168 / 8 = 2Mhz * 168
  
    f(pll out) = f(vco) / PLLP
    f(pll out) = 2Mhz * 168 / 2 = 168Mhz
    
    f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ
    f(USB OTG FS, SDIO, RNG clock output) = 2Mhz * 168 / 7 = 48Mhz
  */

  regVal = (PLLQ << 24)| (PLLP << 16) | (PLLN << 6) | (PLLM);
  
  RCC->PLLCFGR = regVal;

  /*
    turn on PLL
  */
  RCC->CR |= (1U << 24);
  
  /*
    wait until PLL get stable
  */
  while (!(RCC->PLLCFGR & (1U << 24)));


  /*
    PLL can now feed SYSCLK of 168Mhz
    Let's keep AHB/ARM Cortex-M core at maximum capacity i.e. 168Mhz (for STM32F407VG)
    APB1 = 42Mhz (its max capacity for STM32F407VG)
    APB2 = 84Mhz (its max capacity for STM32F407VG)    
  */
  
#define AHB_PRESCALER   0U   /* 0xxx: system clock not divided (HCLK  = SYSCLK) */
#define APB1_PRESCALER  5U   /* 101: AHB clock divided by 4 (PCLK1 = HCLK / 4 = 42Mhz) */
#define APB2_PRESCALER  4U   /* 100: AHB clock divided by 2 (PCLK2 = HCLK / 2 = 84Mhz) */

  regVal = 0;
  regVal = (AHB_PRESCALER << 4) | (APB2_PRESCALER << 13) | (APB1_PRESCALER << 10);

  /*
    switch to PLL as main system clock
  */
  
  regVal |= 0x2;
  RCC->CFGR = regVal;
  
}

