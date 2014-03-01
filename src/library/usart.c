#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "defs.h"

/* =================================================== */
/* Buffers de transmision y recepcion                  */
/* =================================================== */
volatile unsigned char tx_buff[TX_BUFF_SIZE];
volatile unsigned char rx_buff[RX_BUFF_SIZE];
/* =================================================== */

/* =================================================== */
/* Indices de los buffers                              */
/* =================================================== */
volatile unsigned short tx_in;
volatile unsigned short tx_out;		

volatile unsigned short rx_in;
volatile unsigned short rx_out;
/* =================================================== */

//solo implementado el modo asincronico
void USART0Setup(uint16_t baudrate, uint8_t char_size, uint8_t parity, uint8_t stop_bit, uint8_t mode) {

	UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // activamos la recepcion y transmision en el USART0

	UBRR0 = baudrate; // seteamos el prescaler del baudrate
	
	UCSR0C |= ((mode&0x01)<<UMSEL00); // seteamos el modo de operacion syncronico o asyncronico

	UCSR0C |= ((parity&0x03)<<UPM00); // seteamos la cantidad de bits de paridad que vamos a usar

	UCSR0C |= ((stop_bit&0x01)<<USBS0); // seteamos la cantidad de bits de parada que vamos a usar

  UCSR0B |= (((char_size>>2)&0x01)<<UCSZ02);
	UCSR0C |= ((char_size&0x03)<<UCSZ00);
#ifdef USART0_INTERRUPT
    UCSR0B |= (1<<RXCIE0) | (1<<UDRIE0);

    tx_in  = 0;
    tx_out = 0;
    rx_in  = 0;
    rx_out = 0;
    sei();
#endif
}

unsigned char USART0Receive(void){
#ifdef USART0_INTERRUPT
    unsigned char c;
    while(USART0RXBuffLen() == 0);
    c = rx_buff[rx_out];
    rx_out++; 
    rx_out %= RX_BUFF_SIZE;
    return c; // si hay info retorno el dato
#else
	while (IsBitClear(UCSR0A,RXC0)) ;
  return UDR0;
#endif
}

void USART0Transmit(char c){
#ifdef USART0_INTERRUPT
    while((TX_BUFF_SIZE - USART0TXBuffLen()) <= 2);
      // si el buffer esta lleno espero a que la interrupcion libere espacio
    tx_buff[tx_in] = c;
    tx_in++;
    tx_in %= TX_BUFF_SIZE;
    SetBit(UCSR0B,UDRIE0); //habilito la interrupcion
#else
	while (IsBitClear(UCSR0A,UDRE0)); 
	UDR0 = c;
#endif
}

void USART0Putc(char c){
		USART0Transmit(c);
}

void USART0Puts(char *string){
	while (*string)
		USART0Transmit(*string++);
}

void USART0PutsP(char *string){
    char b;

    while ((b = pgm_read_byte(string++)) != 0)
        USART0Transmit(b);
}

#ifdef USART0_INTERRUPT
unsigned char USART0TXBuffLen(void){
    if (tx_in <= tx_out)
      return (tx_out-tx_in+TX_BUFF_SIZE);
    else
      return (tx_in - tx_out);
}

unsigned char USART0RXBuffLen(void){
    if (rx_in <= rx_out)
      return (rx_out-rx_in+RX_BUFF_SIZE);
    else
      return (rx_in - rx_out);
}

ISR(USART_UDRE_vect){ // Vector de interrupcion cuando se vacia el registro de transmision
	if(tx_in != tx_out) { // si el buffer tiene datos los mando por serie
		UDR0 = tx_buff[tx_out];
		tx_out++; // incremento la salida
    tx_out %= TX_BUFF_SIZE;
	}
	else {
		UCSR0B &= ~(1<<UDRIE0);  // si el buffer esta vacio apago la interrupcion
	}
}

ISR(USART_RX_vect){ // Vector de interrupcion cuando se recibe un dato
	rx_buff[rx_in] = UDR0;	// y lo metemos en el buffer
	rx_in++;
  rx_in %= RX_BUFF_SIZE;	// incrementamos la entrada
}

#endif
