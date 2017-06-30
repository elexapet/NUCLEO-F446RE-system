/*
 * STM32F446.h
 *
 * register definitions
 * instruction macros
 */

#pragma once

#include <stdint.h>

//---------------------------------------------

//default CPU frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif //F_CPU

//---------------------------------------------

#define DISABLE_INTERRUPT asm volatile("cpsid i\n\t""cpsid f\n\t");
#define ENABLE_INTERRUPT asm volatile("cpsie i\n\t""cpsie f\n\t");

//----------------------------------------------

#define USART2_BASE 0x40004400
#define USART_SR_OFFSET 0x00
#define USART_DROFFSET 0x04
#define USART_BRROFFSET 0x08
#define USART_CR1OFFSET 0x0C
	#define UE_EN 1<<13
	#define TXEIE_EN 1<<7
	#define RXNEIE_EN 1<<5
	#define TE_EN 1<<3
	#define RE_EN 1<<2
#define USART_CR2OFFSET 0x10
#define USART_CR3OFFSET 0x14
#define USART_GTPROFFSET 0x18

typedef union{
	struct{
		unsigned PE : 1;
		unsigned FE : 1;
		unsigned NF : 1;
		unsigned ORE : 1;
		unsigned IDLE : 1;
		unsigned RXNE : 1;
		unsigned TC : 1;
		unsigned TXE : 1;
		unsigned LBD : 1;
		unsigned CTS : 1;
	};
	uint32_t val;
}USART_SR_t;

typedef union{
	struct{
		unsigned DIV_Fraction : 4;
		unsigned DIV_Mantissa : 12;
	};
	uint32_t val;
}USART_BRR_t;

typedef union{
	struct{
		unsigned SBK : 1;
		unsigned RWU : 1;
		unsigned RE : 1;
		unsigned TE : 1;
		unsigned IDLEIE : 1;
		unsigned RXNEIE : 1;
		unsigned TCIE : 1;
		unsigned TXEIE : 1;
		unsigned PEIE : 1;
		unsigned PS : 1;
		unsigned PCE : 1;
		unsigned WAKE : 1;
		unsigned M : 1;
		unsigned UE : 1;
		unsigned : 1;
		unsigned OVER8 : 1;
	};
	uint32_t val;
}USART_CR1_t;

typedef union{
	struct{
		unsigned ADD : 4;
		unsigned : 1;
		unsigned LBDL : 1;
		unsigned LBDIE : 1;
		unsigned : 1;
		unsigned LBCL : 1;
		unsigned CPHA : 1;
		unsigned CPOL : 1;
		unsigned CLKEN : 1;
		unsigned STOP : 2;
		unsigned LINEN : 1;
	};
	uint32_t val;
}USART_CR2_t;

typedef struct{
	USART_SR_t SR;
	uint32_t DR;
	USART_BRR_t BRR;
	USART_CR1_t CR1;
	USART_CR2_t CR2;
	uint32_t CR3;
	uint32_t GTPR;
}USART_t;


//----------------------------------------------

#define ADC_BASE 0x40012000
#define ADC1_BASE 0x40012000+0x0
#define ADC2_BASE 0x40012000+0x100
#define ADC3_BASE 0x40012000+0x200

#define ADC_SR 0x0
	#define EOC_MASK 1<<1

#define ADC_CR1 0x04
	#define RES_POS 24
	#define EOCIE 1<<5

#define ADC_CR2 0x08
	#define ADON_MASK 1<<0
	#define CONT_MASK 1<<1
	#define SWSTART_MASK 1<<30
	#define EOCS 1<<10

#define SMPR1_OFFSET 0x0C
	#define SMP18_POS 24 //315285684112144480 cycles of ADCCLK

#define SMPR2_OFFSET 0x10
	#define SMP6_MASK 7<<18

#define SQR3_OFFSET 0x34
	#define SQ1_POS 0

#define DR_OFFSET 0x4C
	#define DATA_MASK 0xFFFF

#define CCR_OFFSET 0x04+0x300
	#define TSVREFE_MASK 1<<23
	#define VBATE_MASK 1<<22
	#define ADCPRE_MASK_4 1<<16 //Prescale 4
	#define ADCPRE_MASK_6 2<<16 //Prescale 6
	#define ADCPRE_MASK_8 3<<16 //Prescale 8

//----------------------------------------------

#define RCC_BASE (volatile uint32_t * const)0x40023800
#define RCC_AHB1ENR (volatile uint32_t * const)0x40023830
	#define GPIOC_EN 0b100
	#define GPIOB_EN 0b10
	#define GPIOA_EN 0b1
#define RCC_APB1ENR (volatile uint32_t * const)0x40023840
	#define TIM3_EN 1<<1
	#define TIM12_EN 1<<6
	#define USART2_EN 1<<17
#define RCC_APB2ENR (volatile uint32_t * const)0x40023844
	#define ADC1_EN 1<<8
	#define TIM1_EN 1<<0
	#define TIM8_EN 1<<1

//----------------------------------------------

#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800

#define GPIOx_MODER 0x0
	#define MODERx_AFM 0b10
	#define MODERx_MASK 0b11
	#define MODERx_ANALOG 0b11
	#define MODERx_OUT 0b01
	#define MODERx_IN 0b00
	#define MODER2_AFM (MODERx_AFM << 4)
	#define MODER2_MASK (MODERx_MASK << 4)
	#define MODER3_AFM (MODERx_AFM << 6)
	#define MODER3_MASK (MODERx_MASK << 6)
	#define MODER4_AFM (MODERx_AFM << 8)
	#define MODER4_MASK (MODERx_MASK << 8)
	#define MODER6_MASK (MODERx_MASK << 12)
	#define MODER14_MASK (MODERx_MASK << 28)
	#define MODER14_AFM (MODERx_AFM << 28)
#define GPIOx_PUPDR 0xC
#define GPIOx_IDR 0x10
#define GPIOx_ODR 0x14
#define GPIOx_BSRR 0x18
#define GPIOx_AFRL 0x20
	#define ARFxx_MASK 0b1111
	#define AFRL2_MASK (ARFxx_MASK << 8)
	#define AFRL3_MASK (ARFxx_MASK << 12)
	#define AFRL4_MASK (ARFxx_MASK << 16)
	#define AFRL4_AF2 2 << 16
	#define ARFL2_AF7 7 << 8
	#define ARFL3_AF7 7 << 12
#define GPIOx_AFRH 0x24
	#define ARFxx_MASK 0b1111
	#define AFRH14_MASK (ARFxx_MASK << 24)
	#define AFRH14_AF9 (9 << 24)

typedef union{
	struct{
		unsigned MOD0 : 2;
		unsigned MOD1 : 2;
		unsigned MOD2 : 2;
		unsigned MOD3 : 2;
		unsigned MOD4 : 2;
		unsigned MOD5 : 2;
		unsigned MOD6 : 2;
		unsigned MOD7 : 2;
		unsigned MOD8 : 2;
		unsigned MOD9 : 2;
		unsigned MOD10 : 2;
		unsigned MOD11 : 2;
		unsigned MOD12 : 2;
		unsigned MOD13 : 2;
		unsigned MOD14 : 2;
		unsigned MOD15 : 2;
	};
	uint32_t val;
}GPIO_MODER_t;

typedef union{
	struct{
		unsigned PUPDR0 : 2;
		unsigned PUPDR1 : 2;
		unsigned PUPDR2 : 2;
		unsigned PUPDR3 : 2;
		unsigned PUPDR4 : 2;
		unsigned PUPDR5 : 2;
		unsigned PUPDR6 : 2;
		unsigned PUPDR7 : 2;
		unsigned PUPDR8 : 2;
		unsigned PUPDR9 : 2;
		unsigned PUPDR10 : 2;
		unsigned PUPDR11 : 2;
		unsigned PUPDR12 : 2;
		unsigned PUPDR13 : 2;
		unsigned PUPDR14 : 2;
		unsigned PUPDR15 : 2;
	};
	uint32_t val;
}GPIO_PUPDR_t;

typedef union{
	struct{
		unsigned BS0 : 1;
		unsigned BS1 : 1;
		unsigned BS2 : 1;
		unsigned BS3 : 1;
		unsigned BS4 : 1;
		unsigned BS5 : 1;
		unsigned BS6 : 1;
		unsigned BS7 : 1;
		unsigned BS8 : 1;
		unsigned BS9 : 1;
		unsigned BS10 : 1;
		unsigned BS11 : 1;
		unsigned BS12 : 1;
		unsigned BS13 : 1;
		unsigned BS14 : 1;
		unsigned BS15 : 1;
		unsigned BR0 : 1;
		unsigned BR1 : 1;
		unsigned BR2 : 1;
		unsigned BR3 : 1;
		unsigned BR4 : 1;
		unsigned BR5 : 1;
		unsigned BR6 : 1;
		unsigned BR7 : 1;
		unsigned BR8 : 1;
		unsigned BR9 : 1;
		unsigned BR10 : 1;
		unsigned BR11 : 1;
		unsigned BR12 : 1;
		unsigned BR13 : 1;
		unsigned BR14 : 1;
		unsigned BR15 : 1;
	};
	uint32_t val;
}GPIO_BSRR_t;

typedef union{
	struct{
		unsigned ODR0 : 1;
		unsigned ODR1 : 1;
		unsigned ODR2 : 1;
		unsigned ODR3 : 1;
		unsigned ODR4 : 1;
		unsigned ODR5 : 1;
		unsigned ODR6 : 1;
		unsigned ODR7 : 1;
		unsigned ODR8 : 1;
		unsigned ODR9 : 1;
		unsigned ODR10 : 1;
		unsigned ODR11 : 1;
		unsigned ODR12 : 1;
		unsigned ODR13 : 1;
		unsigned ODR14 : 1;
		unsigned ODR15 : 1;
		unsigned : 16;
	};
	uint32_t val;
}GPIO_ODR_t;

typedef union{
	struct{
		unsigned IDR0 : 1;
		unsigned IDR1 : 1;
		unsigned IDR2 : 1;
		unsigned IDR3 : 1;
		unsigned IDR4 : 1;
		unsigned IDR5 : 1;
		unsigned IDR6 : 1;
		unsigned IDR7 : 1;
		unsigned IDR8 : 1;
		unsigned IDR9 : 1;
		unsigned IDR10 : 1;
		unsigned IDR11 : 1;
		unsigned IDR12 : 1;
		unsigned IDR13 : 1;
		unsigned IDR14 : 1;
		unsigned IDR15 : 1;
		unsigned : 16;
	};
	uint32_t val;
}GPIO_IDR_t;

typedef struct{
	GPIO_MODER_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDER;
	GPIO_PUPDR_t PUPDR;
	GPIO_IDR_t IDR;
	GPIO_ODR_t ODR;
	GPIO_BSRR_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
}GPIO_t;

//NVIC
//----------------------------------------------

#define NVIC_BASEADDR 0xE000E100
#define NVIC_ISER0 0xE000E100+0x0
#define NVIC_ISER1 0xE000E100+0x4
#define NVIC_ISER2 0xE000E100+0x8

typedef struct{
	uint32_t ISER0; //IRQ 0-31
	uint32_t ISER1; //IRQ 32-63
	uint32_t ISER2; //IRQ 64-95
}NVIC_t;

//ICSR
//----------------------------------------------

#define ICSR_BASEADDR 0xE000ED04

typedef struct{
	unsigned VECTACTIVE : 9;
	unsigned : 2;
	unsigned RETTOBASE : 1;
	unsigned VECTPENDING : 7;
	unsigned : 3;
	unsigned ISRPENDING : 1;
	unsigned : 2;
	unsigned PENDSTCLR : 1;
	unsigned PENDSTSET : 1;
	unsigned PENDSVCLR : 1;
	unsigned PENDSVSET : 1;
	unsigned : 2;
	unsigned NMIPENDSET : 1;
}ICSR_t;

//SYSTICK
//----------------------------------------------

#define STK_BASE 0xE000E010
	#define STK_CTRL 0x00
	#define STK_LOAD 0x04
	#define STK_VAL 0x08
	#define STK_CALIB 0x0C


typedef struct{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;
}SYSTICK;

//TIMERS
//----------------------------------------------
#define TIM1_BASE 0x40010000
#define TIM8_BASE 0x40010400


#define TIM12_BASE 0x40001800 //used for IR

#define TIM3_BASE 0x40000400 //used for speaker
#define TIM4_BASE 0x40000800

	#define CR1_OFFSET 0x0
		#define CR1_CEN 0b1
	#define CCER_OFFSET 0x20
		#define CCER_CC1NP 1<<3
		#define CCER_CC1P 1<<1
		#define CCER_CC1E 1<<0
	#define PSC_OFFSET 0x28
	#define ARR_OFFSET 0x2C
	#define DIER_OFFSET 0x0C
	#define EGR_OFFSET 0x14
	#define CCMR1_OFFSET 0x18
		#define CC1S_MASK 0b11
		#define CC1S_IC1_TI1 0b01
		#define IC1PSC_MASK (0b11 << 2)
		#define OC1M_TOGGLE (0b011 << 4)
		#define OC1M_MASK (0b111 << 4)
	#define CCR1_OFFSET 0x34
		#define CCR1_CC1IE_EN 1<<1


typedef union{
	struct{
		unsigned CEN : 1;
		unsigned UDIS : 1;
		unsigned URS : 1;
		unsigned OPM : 1;
		unsigned DIR : 1;
		unsigned CMS : 2;
		unsigned ARPE : 1;
		unsigned CKD : 2;
		unsigned : 22;
	};
	uint32_t val;
}TIMx_CR1;

typedef struct{
	TIMx_CR1 CR1;
	uint32_t CR2;
	uint32_t SMCR;
	uint32_t DIER;
	uint32_t SR;
	uint32_t EGR;
	uint32_t CCMR1;
	uint32_t CCMR2;
	uint32_t CCER;
	uint32_t CNT;
	uint32_t PSC;
	uint32_t ARR;
	unsigned : 32;
	uint32_t CCR1;
	uint32_t CCR2;
	uint32_t CCR3;
	uint32_t CCR4;
	unsigned : 32;
	uint32_t DCR;
	uint32_t DMAR;
}TIM2_5;

//----------------------------------------------
//----------------------------------------------

volatile GPIO_t * const GpioA;
volatile GPIO_t * const GpioB;
volatile GPIO_t * const GpioC;

