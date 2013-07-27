#include <avr/io.h>
#include <util/delay.h>
#include "board.h"
#include "cubeled3.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include "cubemodes.h"

void ColumnAllOn() {
  ColumnOn(0);
  ColumnOn(1);
  ColumnOn(2);
  ColumnOn(3);
  ColumnOn(4);
  ColumnOn(5);
  ColumnOn(6);
  ColumnOn(7);
  ColumnOn(8);
  return;
}

void ColumnAllOff() {
  ColumnOff(0);
  ColumnOff(1);
  ColumnOff(2);
  ColumnOff(3);
  ColumnOff(4);
  ColumnOff(5);
  ColumnOff(6);
  ColumnOff(7);
  ColumnOff(8);
  return;
}

volatile bool frameEnded;

// Tenemos 3 filas 0 1 y 2, siendo 0 la fila superior.
// Por cada fila, los 9 bits menos significativos
// corresponden a los estados de las columnas.
//
// Visto desde arriba las columnas son
// 0 1 2
// 3 4 5
// 6 7 8

volatile uint16_t buffer[ROWS];
uint16_t * pbuff;

volatile uint8_t mode;


void setup (void) {
  SetBit(DDR_C0,C0_NUM);
  SetBit(DDR_C1,C1_NUM);
  SetBit(DDR_C2,C2_NUM);
  SetBit(DDR_C3,C3_NUM);
  SetBit(DDR_C4,C4_NUM);
  SetBit(DDR_C5,C5_NUM);
  SetBit(DDR_C6,C6_NUM);
  SetBit(DDR_C7,C7_NUM);
  SetBit(DDR_C8,C8_NUM);
  SetBit(DDR_R0,R0_NUM);
  SetBit(DDR_R1,R1_NUM);
  SetBit(DDR_R2,R2_NUM);
  ColumnAllOff();
  RowOff(1);
  RowOff(2);
  RowOff(0);


  frameEnded = false;
  pbuff = &buffer[0];

  TCCR0A = (1<<WGM01) | (0<<WGM00);
  OCR0A = 50; //Tire numero al azar, calcular
  TIMSK0 = (1<<OCIE0A);
  TCCR0B = (1<<CS02) | (0<<CS01) | (1<<CS00);

  sei();

}



int main(void) {
  setup();
  
  mode = 0;
  
  while(1){

    cubeModes(mode);
    while (modeEnded == false);
    mode++;
    if (mode == TOTAL_MODES) mode = 0;
  }



}

void RoWOff(uint8_t r){
  if (r==0) RowOff(0);
  else if (r==1) RowOff(1);
  else if (r==2) RowOff(2);
}

void RoWOn(uint8_t r){
  if (r==0) RowOn(0);
  else if (r==1) RowOn(1);
  else if (r==2) RowOn(2);
}

void ColumNOn(uint8_t c){
  if (c==0) ColumnOn(0);
  else if (c==1) ColumnOn(1);
  else if (c==2) ColumnOn(2);
  else if (c==3) ColumnOn(3);
  else if (c==4) ColumnOn(4);
  else if (c==5) ColumnOn(5);
  else if (c==6) ColumnOn(6);
  else if (c==7) ColumnOn(7);
  else if (c==8) ColumnOn(8);
}

void ColumNOff(uint8_t c){
  if (c==0) ColumnOff(0);
  else if (c==1) ColumnOff(1);
  else if (c==2) ColumnOff(2);
  else if (c==3) ColumnOff(3);
  else if (c==4) ColumnOff(4);
  else if (c==5) ColumnOff(5);
  else if (c==6) ColumnOff(6);
  else if (c==7) ColumnOff(7);
  else if (c==8) ColumnOff(8);
}


void Columns(uint16_t data){
  uint8_t i;

  for(i=0;i<COLUMNS;i++) {
    if (data & 0x0001) ColumNOn(i);
    else ColumNOff(i);
    data = data >> 1;
  }
}

ISR(TIMER0_COMPA_vect){

  static uint8_t row = 0;
  
  frameEnded = false;
  RoWOff(row);
  row++;
  row = row % 3;
  RoWOn(row);

  Columns(buffer[row]);
  if (row == 2) frameEnded = true;
}

/*
ISR(TIMER0_OVF_vect){
  Led2Toggle();
  static bool test2 = false;

  if (test2) {
    ColumnAllOn();
    test2 = false;
  }
  else {
    ColumnAllOff();
    test2 = true;
  }
}
*/
