#include "uart.h"

static volatile USART_t * const pUSART2 = (USART_t *)USART2_BASE;

static volatile CharRingBuffer_t txBuffer = {0,0};
static volatile CharRingBuffer_t rxBuffer = {0,0};

void USART2_init(unsigned baudRate)
{
	// Enable clocks for GPIOA and USART2
	*(RCC_AHB1ENR) |= GPIOA_EN;
	*(RCC_APB1ENR) |= USART2_EN;

	//select AF7 on PA2 and PA3
	GpioA->AFRL = (GpioA->AFRL & ~(AFRL2_MASK | AFRL3_MASK)) | ARFL2_AF7 | ARFL3_AF7;
	//enable AF on PA2 and PA3
	GpioA->MODER.MOD2 = MODERx_AFM;
	GpioA->MODER.MOD3 = MODERx_AFM;

	//set baud rate
	pUSART2->BRR.val = F_CPU/baudRate;

	//enable TX & RX & USART & Interrupts
	pUSART2->CR1.val = 0;
	pUSART2->CR1.UE = 1;
	pUSART2->CR1.RE = 1;
	pUSART2->CR1.TE = 1;
	pUSART2->CR1.RXNEIE = 1;

	//clear other control registers
	pUSART2->CR2.val = 0;
	pUSART2->CR3 = 0;
	pUSART2->DR = 0;

	//for standard I/O
	setvbuf(stdout, NULL, _IONBF, 0);

	//enable IRQ #38 in NVIC
	((NVIC_t*)NVIC_BASEADDR)->ISER1 |= 1 << 6;
}

char USART2_get()
{
	//read data from buffer
	char data = RingBuffer_Get(&rxBuffer);
	if (data == '\r') data = '\n';
	return data;
}
void USART2_put(char data)
{
	//copy data to output buffer
	RingBuffer_Put(&txBuffer, data);

	//enable Tx interrupt
	pUSART2->CR1.TXEIE = 1;
}

__attribute__((isr))
void USART2_IRQHandler(void)
{
	if (pUSART2->SR.RXNE)
	{
		//save received byte to buffer
		char data = pUSART2->DR;
		RingBuffer_PutNoBlock(&rxBuffer, data);
		//echo back
		RingBuffer_PutNoBlock(&txBuffer, data);
		pUSART2->CR1.TXEIE = 1;
	}
	if (pUSART2->CR1.TXEIE)
	{
		if (RingBuffer_IsEmpty(&txBuffer)) pUSART2->CR1.TXEIE = 0;
		else pUSART2->DR = RingBuffer_GetNoBlock(&txBuffer);
	}
}
