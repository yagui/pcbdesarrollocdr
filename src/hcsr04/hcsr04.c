#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

uint8_t volatile contador;
uint8_t volatile ciclo;


void chartostring(uint8_t numero, char * cadena);
void uinttostring(uint16_t numero, char * cadena);

void uinttostring(uint16_t numero, char * cadena) {
  cadena[0] = numero / 10000 + 0x30;
  numero = numero % 10000;
  cadena[1] = numero / 1000 + 0x30;
  numero = numero % 1000;
  cadena[2] = numero / 100 + 0x30;
  numero = numero % 100;
  cadena[3] = numero / 10 + 0x30;
  numero = numero % 10;
  cadena[4] = numero + 0x30;
  cadena[5] = '\n';
  cadena[6] = 0;
}

void chartostring(uint8_t numero, char * cadena) {
  cadena[0] = numero / 100 + 0x30;
  numero = numero % 100;
  cadena[1] = numero / 10 + 0x30;
  numero = numero % 10;
  cadena[2] = numero + 0x30;
  cadena[3] = '\n';
  cadena[4] = 0;
}

#define TRIGGERPORT    PORTD
#define TRIGGERDDR     DDRD
#define TRIGGERNUM     7
#define SetTrigger() {TRIGGERPORT |= (1<<TRIGGERNUM);}
#define ClearTrigger() {TRIGGERPORT &= ~(1<<TRIGGERNUM);}
#define InitTrigger() {TRIGGERDDR |= (1<<TRIGGERNUM);}
uint32_t medicion;

int main(void){
  char buffer[6];
  DDRC = (1<<PC3)|(1<<PC2);
  DDRD |= (1<<PD4);
  InitTrigger();

//  TCCR1A = (0<<WGM11)|(0<<WGM10);
//  TCCR1B = (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12);
  PCMSK0 = (1<<PCINT0); //INT0 enable
  USART0Setup();

  ClearTrigger();
  PORTC |= (1<<PC2);
  PORTC |= (1<<PC3);
  PORTD |= (1<<PD4);

  _delay_ms(5);

  PCIFR |= (1<<PCIF0);
  TIFR1 |= (1<<ICF1);
  
  sei();
  
  USART0PutsP(PSTR("Initializing ...\n"));
  _delay_ms(2500);
  
  while(1){

    SetTrigger();
    _delay_us(50);
    ClearTrigger();

    PCIFR |= (1<<PCIF0);
    PCICR |= (1<<PCIE0);

    
    PORTC &= ~(1<<PC3);
    _delay_ms(500);
    PORTC |= (1<<PC3);
    _delay_ms(500);

    uinttostring(medicion,buffer);
    USART0PutsP(PSTR("Distance = "));
    USART0Puts(buffer);
  }
}


ISR(PCINT0_vect){
  PINC |= (1<<PC2);
  PCICR &= ~(1<<PCIE0);

  TCNT1 = 0x0000;

  TIFR1 |= (1<<ICF1);
  TIMSK1 |= (1<<ICIE1);

  TCCR1B |= 0x02; //prendo el timer
}


ISR(TIMER1_CAPT_vect){

  medicion = ICR1;
  TCCR1B &= ~(0x07);
  TIMSK1 &= ~(1<<ICIE1);

  PIND |= (1<<PD4);

}
