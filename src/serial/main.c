#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

uint8_t volatile contador;
uint8_t volatile ciclo;


void chartostring(uint8_t numero, char * cadena);

void chartostring(uint8_t numero, char * cadena) {
  cadena[0] = numero / 100 + 0x30;
  numero = numero % 100;
  cadena[1] = numero / 10 + 0x30;
  numero = numero % 10;
  cadena[2] = numero + 0x30;
  cadena[3] = '\n';
  cadena[4] = 0;
}

int main(void){
  DDRC = (1<<PC3);
  
  contador = 254;
  ciclo = 255;
  TIMSK1= 0x01;  
  USART0Setup();
  TCCR1B = 0x01;
  while(1){
    _delay_ms(500);
    PORTC |= (1<<PC3);
    _delay_ms(500);
    PORTC &= ~(1<<PC3);
  }
}


ISR(TIMER1_OVF_vect){
  char numeroascii[4];
  contador++;

  if (!contador) {
    ciclo++;
    chartostring(ciclo,numeroascii);
    USART0PutsP(PSTR("Ciclo "));
    USART0Puts(numeroascii);
  }
}
