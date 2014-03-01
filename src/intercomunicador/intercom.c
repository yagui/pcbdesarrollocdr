#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ADC_MODO_INT_
#define ADC_MODO_8BITS_

#define TICKER_MS   10

#include "board.h"
#include "usart.h"
#include "adc.h"
#include <stdbool.h>

volatile bool newdata;
volatile char data;

int i;
void setup (void) {
  Led1Init();
  Led2Init();
  Led3Init();
  Led1Off();
  Led2Off();
  Led3Off();
	// CTC with OCR1A as TOP
	//TCCR1A = (1 << WGM11);
	// clk_io/1024 (From prescaler)
	TCCR1B = ((1<<WGM12)|(1 << CS12) | (0 << CS11)| (1 << CS10));
	// Generate an interrupt every 'TICKER_MS' ms
	OCR1A = (1*TICKER_MS);
	// Enable Timer/Counter0 Compare Match A interrupt
	TIMSK1 |= (1 << OCIE1A);
  i = 0;

  TCCR2A = ( (1<<COM2A1) | (1<<WGM21) | (1<<WGM20) );
  OCR2A = 0;
  TCCR2B = ( (0<<CS22) | (0<<CS21) | (1<<CS20) );

  ADCSetup(ADCPS4,LDRADCCH);
  
  USART0Setup(USART_BAUDRATE_115200,USART_CHARSIZE_8BIT,USART_PARITY_DIS,USART_STOP_1BIT,USART_MODE_ASYNC);

  newdata = false;
  sei();
 
}

uint8_t tempc=0;

int main(void) {

  setup();
  
  // inicio un timer

  while(1){
    if (newdata) {

      //USART0Putc(data);
      USART0Putc(tempc++);
      newdata = false;
    }
    if (IsNewRxData()) {
      
      OCR2A = USART0Receive();
    }
  }


}

ISR(TIMER1_COMPA_vect){
  i++;
  if (i==1) {//iniciar conversion
    Led2Toggle();
    ADCInitConvertion();
    i=0;
  }
}

ISR(ADC_vect) {
  Led3Toggle();
  // termino la conversion
  data = ADCGetData();
  // se envía por serie
  newdata = true;
  SetBit(ADCSRA,ADIF);
}
