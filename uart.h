//#define FOSC 1843200// Clock Speed
#define FOSC 4915200 // Clock Speed
#define BAUD 76800
unsigned int MYUBRR = FOSC/16/BAUD-1;

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);