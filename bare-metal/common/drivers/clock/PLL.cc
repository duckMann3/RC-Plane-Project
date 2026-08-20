/* TASK: Configure System Clock to 100MHz */

/* Base Addresses:
        RCC:   0x40023800
        FLASH: 0x40023C00
        PWR:   0x400070000
*/

/* Flash Register:
    > FLASH_ACR (0x00):
        ICEN, DCEN, PRFTEN, LATENCY
*/

/* RCC Register:
    > RCC_CR (0x00): 
        PLLON, HSEON
    > RCC_PLLCFGR (0x04): 
        PLLQ, PLLSRC, PLLP, PLLN, PPLM
    > RCC_CFGR (0x08): 
        HPRE, PPRE1 
    > RCC_APB1ENR (0x40):
        PWREN
*/

//& Steps:
/*
    ~ 1. ENABLE HSE and wait for the HSE to become READY.
    ~ 2. Set the POWER ENABLE CLOCK & VOLTAGE REGULATOR.
    ~ 3. Config the FLASH PREFETCH and the LATENCY Related Settings. 
    ~ 4. Config the PRESCALARS HCLK, PCLK1, PLCK2.
    ~ 5. Configure the MAIN PLL.
    ~ 6. Enable the PLL and wait for it to become READY.
    ~ 7. Select the Clock Source and wait for it to be set.
*/

//& Reset & Clock Control (RCC) Registers:
#define RCC_BASE 0x40023800

#define RCC_CR   (*(volatile unsigned long*)(RCC_BASE + 0x00))
#define HSEON    (1 << 16) // Bit 16, 1: ON
#define HSERDY   (1 << 17) // Bit 17: 1: is READY 
#define PLLON    (1 << 24) // Bit 24, 1: ON, NOTE: turn on at the end of config
#define PLLRDY   (1 << 25) // Bit 25, 0: PLL Unlocked

#define RCC_CFGR   (*(volatile unsigned long*)(RCC_BASE + 0x00))
#define HPRE_DIV1  (0 << 4)  // Bits 7:4, 
#define PPRE1_DIV2 (2 << 10) // Bits 12:10,  
#define PPRE2_DIV1 (0 << 3)  // Bits 15:13, 

#define RCC_PLLCFGR (*(volatile unsigned long*)(RCC_BASE + 0x04))
#define PPLM        25
#define PLLN        400
#define PLLP        1 // 01 corresponds to PLLP = 4
#define PLLSRC      (1 << 22) 
#define SW          (2 << 0) // Bits 1:0, 10: PLL Selected as System Clock (Set PLL as Clock Source)
#define SWS         (2 << 2) // Bits 3:0, 10: PLL is used as system clock (Set PLL as Clock Source Status)

#define RCC_APB1ENR (*(volatile unsigned long*)(RCC_BASE + 0x40))
#define PWREN (1 << 28) // Bit 28, 1: clock enable 

//& Flash Interface Registers:
#define FLASH_BASE 0x40023C00
#define FLASH_ACR  (*(volatile unsigned long*)(FLASH_BASE + 0x00))
#define DCEN    (1 << 10) // Bit 10, 1: enabled
#define ICEN    (1 << 9) // Bit 9, 1: enabled
#define PRFTEN  (1 << 8) // Bit 8, 1: enabled
#define LATENCY_3WS (3 << 0) // Bit 3:0, 0011: three wait states

//& Power Control:
#define PWR_BASE 0x40007000  
#define PWR_CR   (*(volatile unsigned long*)(PWR_BASE))  
#define VOS      (3 << 14)

void SystemClock_Config(void);

void SystemClock_Config(void) {
    // 1. Set Control Register:
    RCC_CR |= HSEON;
    while(!(RCC_CR&HSERDY));
    // 2. Set Power Enable & Voltage Regulator:
    RCC_APB1ENR |= PWREN;
    PWR_CR |= VOS;
    // 3. Configure Flash Register:
    // Set flash latency, enable Data, Instruction, & Prefetch Cache.
    FLASH_ACR = DCEN | ICEN | PRFTEN | LATENCY_3WS;
    // 4. Set Prescaler:
    RCC_CFGR |= HPRE_DIV1;  // AHB PR
    RCC_CFGR |= PPRE1_DIV2; // APB1 PR
    RCC_CFGR |= PPRE2_DIV1; // APB2 PR
    // 5. Set RCC PLL Configuration Register:
    RCC_PLLCFGR |= (PPLM << 0) | (PLLN << 16) | (PLLP << 16);
    RCC_PLLCFGR |= PLLSRC;
    // 6. Enable PLL:
    RCC_CR |= PLLON; 
    while(!(RCC_CR&PLLRDY));
    // 7. Set Clock Source:
    RCC_CFGR |= SW;
    while((RCC_CFGR&SWS) != SW);
}