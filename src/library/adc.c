#include <avr/io.h>
#include <util/delay.h>
#include "board.h"
#include "adc.h"


void ADCSelectChannel(char channel){
	//Seteamos el pin de ADC que se va a usar, como entrada	
	ClearBit(PORT_LDR, LDR_NUMBER); 

	ADMUX = ((ADMUX & 0xE0) | (0x07 & channel));

}


void ADCSetup(char prescaler, char channel){
  /****** Configuracion ADC *****/
  
  // Para elegir la referencia se usan los bits REFS1 y REFS0. Los cuales deben
  // estar en 0 y 1 respectivamente para usar la tension de alimentacion como
  // referencia
	SetBit(ADMUX, REFS0); 	
	ClearBit(ADMUX, REFS1); 	

#ifdef ADC_MODO_8BITS
  SetBit(ADMUX, ADLAR);
#endif


	//Usamos la resolucion de 8 bits(datasheet ATMega 8: pagina 208).
  //ClearBit(ADMUX, ADLAR); 

	// Seleccionamos el ADC, donde tenemos conectado nuestro dispositivo (hay
  // que pasarlo como argumento a ADC_setup())
	ADCSelectChannel(channel);
  
	ADCSelectPrescaler(prescaler);
	//Utilizamos el modo "Single Conversion"

#ifdef ADC_MODO_INT
	// Habilitamos la interrupcion de finalizacion de conversion AD
	SetBit(ADCSRA, ADIE);
	//Utilizamos el modo "Free Run"
#else
	//Deshabilitamos la interrupcion de finalizacion de conversion AD
	//ClearBit(ADCSRA, ADIE);
#endif

  // Prende los conversores seteando bit en el ADCSRA
  SetBit(ADCSRA, ADEN);
}

#ifdef ADC_MODO_8BITS
char ADCGetData(){
  return ADCH;
}
#else
short ADCGetData(){
  short temp;
  temp = ADCL;
  temp = (ADCH << 8) | temp;
  return temp;
}
#endif


#ifdef ADC_MODO_8BITS
  char ADCSingleConvertion() {
#else
  short ADCSingleConvertion() {
#endif
    ADCInitConvertion();

#ifdef ADC_MODO_INT
    return 0;
#else
    while( (ADCSRA & (1<<ADIF)) == 0);
    // borramos el flag
    ADCSRA |= (1<<ADIF);
    return ADCGetData();
#endif
}

