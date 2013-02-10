#include <avr/io.h>
#include <util/delay.h>
#include "board.h"
#include "cubeled3.h"

void setup (void) {
  SetBit(DDR_C1,C1_NUM);
  SetBit(DDR_C2,C2_NUM);
  SetBit(DDR_C3,C3_NUM);
  SetBit(DDR_C4,C4_NUM);
  SetBit(DDR_C5,C5_NUM);
  SetBit(DDR_C6,C6_NUM);
  SetBit(DDR_C7,C7_NUM);
  SetBit(DDR_C8,C8_NUM);
  SetBit(DDR_C9,C9_NUM);
  SetBit(DDR_R1,R1_NUM);
  SetBit(DDR_R2,R2_NUM);
  SetBit(DDR_R3,R3_NUM);
}

void ColumnAllOn() {
  ColumnOn(1);
  ColumnOn(2);
  ColumnOn(3);
  ColumnOn(4);
  ColumnOn(5);
  ColumnOn(6);
  ColumnOn(7);
  ColumnOn(8);
  ColumnOn(9);
  return;
}

void ColumnAllOff() {
  ColumnOff(1);
  ColumnOff(2);
  ColumnOff(3);
  ColumnOff(4);
  ColumnOff(5);
  ColumnOff(6);
  ColumnOff(7);
  ColumnOff(8);
  ColumnOff(9);
  return;
}

int main(void) {
  setup();

  while(1){
    ColumnAllOn();
    RowOn(1);
    RowOff(2);
    RowOff(3);
    _delay_ms(250);
    RowOn(2);
    RowOff(1);
    _delay_ms(250);
    RowOn(3);
    RowOff(2);
    _delay_ms(250);
    RowOff(3);
    _delay_ms(100);
    ColumnAllOff();

    ColumnOn(1);
    _delay_ms(250);
    ColumnOff(1);
    ColumnOn(2);
    _delay_ms(250);
    ColumnOff(2);
    ColumnOn(3);
    _delay_ms(250);
    ColumnOff(3);
    ColumnOn(4);
    _delay_ms(250);
    ColumnOff(4);
    ColumnOn(5);
    _delay_ms(250);
    ColumnOff(5);
    ColumnOn(6);
    _delay_ms(250);
    ColumnOff(6);
    ColumnOn(7);
    _delay_ms(250);
    ColumnOff(7);
    ColumnOn(8);
    _delay_ms(250);
    ColumnOff(8);
    ColumnOn(9);
    _delay_ms(250);
    ColumnOff(9);
    _delay_ms(250);
  }
}

