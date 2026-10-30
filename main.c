#include <stdint.h>

#define RCC_BASE          0x40021000
#define RCC_APB2ENR       (*(volatile uint32_t *)(RCC_BASE + 0x18))

#define GPIOA_BASE        0x40010800
#define GPIOA_CRL         (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR         (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_IDR         (*(volatile uint32_t *)(GPIOA_BASE + 0x08))

#define GPIOC_BASE        0x40011000
#define GPIOC_CRH         (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR         (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))

#define RCC_APB2ENR_IOPAEN (1 << 2)
#define RCC_APB2ENR_IOPCEN (1 << 4)


void delay(volatile uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}

int __attribute__((noreturn)) main(void) {
    
    RCC_APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN);

    
    GPIOC_CRH = (GPIOC_CRH & ~(0xF << 20)) | (0x1 << 20);

    
    GPIOA_CRL = (GPIOA_CRL & ~(0xFF << 0)) | (0x88 << 0);
    
   
    GPIOA_ODR |= (1 << 0) | (1 << 1);

    
    uint32_t half_period = 1500000; 
    volatile uint32_t i;

    while (1) {
        
        if (!(GPIOA_IDR & (1 << 0))) { 
            for(i = 0; i < 30000; i++); 
            if (!(GPIOA_IDR & (1 << 0))) {
                
                if (half_period > 25000) { 
                    half_period /= 2;
                }
                while (!(GPIOA_IDR & (1 << 0))); 
            }
        }

 
        if (!(GPIOA_IDR & (1 << 1))) { 
            for(i = 0; i < 30000; i++); 
            if (!(GPIOA_IDR & (1 << 1))) {

                if (half_period < 96000000) { 
                    half_period *= 2;
                }
                while (!(GPIOA_IDR & (1 << 1))); 
            }
        }

        
        GPIOC_ODR &= ~(1 << 13); 
        delay(half_period);
        
        GPIOC_ODR |= (1 << 13);  
        delay(half_period);
    }
}