#include "stm32f10x.h" 

#define SQUARE_WAVE_PERIOD_MS 10000

void SystemClock_Config(void);
void GPIO_Init(void);
void OutputSquareWave(int high);
void DelayMs(uint32_t ms);

int main(void) {
    SystemClock_Config();
    GPIO_Init();

    while (1) {
        OutputSquareWave(1);
        DelayMs(SQUARE_WAVE_PERIOD_MS / 2);
        OutputSquareWave(0);
        DelayMs(SQUARE_WAVE_PERIOD_MS / 2);
    }
}

void SystemClock_Config(void) {z
    // Enable High Speed Internal Clock (HSI)
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));  // Wait for HSI Ready

    RCC->CFGR &= ~RCC_CFGR_SW;          // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_HSI;       // Select HSI as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); // Wait for HSI used as system clock

}

void GPIO_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // Enable clock for GPIOA
	  GPIOA->CRL &= ~(0xFFFFFFFF);
    GPIOA->CRL = 0x33333333;  // Configure all pins of GPIOA (PA0-PA7) as push-pull output max speed 50 MHz
}

void OutputSquareWave(int high) {
    if (high) {
        GPIOA->BSRR = 0x00FF;  // Set PA0-PA7
    } else {
        GPIOA->BSRR = 0xFF0000;  // Reset PA0-PA7
    }
}

void DelayMs(uint32_t ms) {
    SysTick->LOAD = 8000 - 1;  // Assuming 8 MHz HSI clock, 1ms count down
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;

    for(uint32_t i = 0; i < ms; i++) {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));  // Wait until count flag is set
    }

    SysTick->CTRL = 0;  // Disable SysTick
}

