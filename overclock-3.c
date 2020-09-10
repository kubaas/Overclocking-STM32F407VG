#include "stm32f4xx.h"
#define PLL_M           8
#define PLL_N           400


static uint32_t cnt = 0;

void SysTick_Handler(void)
{
	if(cnt)
		cnt--;
}

int main()
{
   //Reseting registers for RCC
    RCC->CR = (uint32_t)0x00000083;
    RCC->CFGR = (uint32_t)0x00000000;
    RCC->PLLCFGR = (uint32_t)0x24003010;

    //HSE
    RCC->CR |= RCC_CR_HSEON;  //Turning on HSE
		while(!(RCC->CR & RCC_CR_HSERDY)){};    //Loop waiting for turning on HSE
     
	
    //Setting clock PLL    
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;  //Reset value of PLLM
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;  //Reset value of PLLN	
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;  //Reset value of PLLP, after that default value is 2			
		RCC->PLLCFGR = PLL_M | (PLL_N << 6) | RCC_PLLCFGR_PLLSRC_HSE; //Setting values of PLLM, PLLN. Timing source as HSE

    RCC->CR |= RCC_CR_PLLON;  //Turn on PLL
    while(!(RCC->CR & RCC_CR_PLLRDY)){};  //Loop waiting for turning on PLL
          
		// Configure Flash prefetch, Instruction cache, Data cache and wait state
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
			
    RCC->CFGR |= RCC_CFGR_SW_PLL;  //Setting PLL as system clock
    while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)){};  //Loop waiting for turning on PLL as system clock
				
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;
		GPIOD->MODER|=GPIO_MODER_MODER12_0;
		GPIOD->MODER|=GPIO_MODER_MODER13_0;
		GPIOD->MODER|=GPIO_MODER_MODER14_0;
		GPIOD->MODER|=GPIO_MODER_MODER15_0;
		
		long int SysClk=200000000;
		SysTick->LOAD = SysClk/1000;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;	
			
   
    while(1) {
if (!cnt)
	{
GPIOD->ODR ^= GPIO_ODR_ODR_12;
GPIOD->ODR ^= GPIO_ODR_ODR_13;
GPIOD->ODR ^= GPIO_ODR_ODR_14;
GPIOD->ODR ^= GPIO_ODR_ODR_15;
		cnt=1000;
	}
}
		return(1);
}