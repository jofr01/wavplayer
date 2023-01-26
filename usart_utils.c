#include "main.h"
#include "usart_utils.h"

void USART_init(){
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
	GPIOA->AFR[0] |= ((7<<GPIO_AFRL_AFSEL2_Pos)|(7<<GPIO_AFRL_AFSEL3_Pos)); 	//AF 7 = USART2
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
	RCC->APB1ENR1 |= (RCC_APB1ENR1_USART2EN);
	USART2->CR1 &= 0x0;		//1 start bit, 8 data bits, Oversampling 16
	USART2->CR2 &= 0x0;		//1 stop bit, LSB first
	USART2->BRR = 35;		//baudrate 115200 with oversampling 16
	USART2->CR1 |= (1<<0)|(1<<2)|(1<<3)|(1<<5); //USART, RX, TX, TX interrupt enabled
	USART2->ISR &= ~(USART_ISR_RXNE); //clear isr flag
	NVIC->ISER[1] |= (1<<(USART2_IRQn-32)); //enable interrupt vector
	__enable_irq();
}

void A7_print(){
	USART_print_esc("[2J");
	USART_print_esc("[H");
	USART_print_esc("[3B");		//down 3 lines
	USART_print_esc("[5C");		//right 5 spaces
	USART_print("All good students read the");
	USART_print_esc("[1B");		//down 1 line
	USART_print_esc("[21D");	//go left 21 spaces
	USART_print_esc("[5m");		//blinking mode
	USART_print("Reference Manual");
	USART_print_esc("[H");		//move cursor to top left corner
	USART_print_esc("[0m");		//disable character attributes
	USART_print("Input: ");
}

void USART_print_esc (char* string){
	while(!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = (uint8_t) 27;
	for (int i = 0; string[i] != NULL; i++){
		while(!(USART2->ISR & USART_ISR_TXE));
		USART2->TDR = string[i];
	}
}

void USART_print (char* string){
	for (int i = 0; string[i] != '\0'; i++){
		while(!(USART2->ISR & USART_ISR_TXE));
		USART2->TDR = string[i];
	}
}


