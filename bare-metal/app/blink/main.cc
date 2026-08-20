/* TASK
    > Set relevant registers for GPIO (specifically on-board LED).
    > Blink LED using busy wait delay.
*/

/* Configuration:
    > Enable GPIO clock via RCC reg
    > Configure target pin via MODER reg
    > Toggle output via ODR
*/

/* Reset & Clock Control:
    > RCC -> AHB1ENR
    > Desired: PC13
    > Bit 2 GPIOCEN:
        - 0: disable
        - 1: Enable
*/

#define RCC_BASE   0x40023800
#define GPIOC_BASE 0x40020800

#define RCC_AHB1ENR (*(volatile unsigned long*)(RCC_BASE + 0x30))
#define GPIOC_MODER (*(volatile unsigned long*)(GPIOC_BASE + 0x00))
#define GPIOC_ODR   (*(volatile unsigned long*)(GPIOC_BASE + 0x14))

void delay(volatile unsigned int time);

int main(void) {
    // 1. Enable Clock for GPIOC (Bit 2)
    RCC_AHB1ENR |= (1 << 2); 
    // 2. Configure PC13 as General-Purpose Output Mode (01):
    GPIOC_MODER &= ~(3 << (13 * 2));
    GPIOC_MODER |= (1 << (13 * 2));

    while(1) {
        // 3. Toggle Pin PC13 using Output Data Register:
        GPIOC_ODR ^= (1 << 13);
        delay(500000);
    }
    return 0;
}

void delay(volatile unsigned int time) {
    while(time--);
}