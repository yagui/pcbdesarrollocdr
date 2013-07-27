#include "cubemodes.h"

volatile bool modeEnded;

#define DELAY_NIBBLE_1 150
#define DELAY_LG 240


extern volatile uint16_t buffer[ROWS];

void modeNibble(){
    buffer[0] = (1<<0);
    buffer[1] = 0;
    buffer[2] = 0;
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<1);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<2);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<5);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<4);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<3);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<6);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<7);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = (1<<8);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[0] = 0;
    buffer[1] = (1<<8);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<7);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<6);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<3);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<4);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<5);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<2);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<1);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = (1<<0);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[1] = 0;
    buffer[2] = (1<<0);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<1);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<2);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<5);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<4);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<3);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<6);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<7);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = (1<<8);
    _delay_ms(DELAY_NIBBLE_1);
    buffer[2] = 0;
    buffer[1] = (1<<4);
    _delay_ms(DELAY_NIBBLE_1);
}


void modeGuir(void){
  buffer[0] = (1<<0) | (1<<8);
  buffer[1] = (1<<1) | (1<<3) | (1<<5) | (1<<7);
  buffer[2] = (1<<2) | (1<<6);
	_delay_ms(DELAY_LG);
  buffer[0] = 0;
  buffer[1] = (1<<0) | (1<<1) | (1<<2) | (1<<3)| (1<<5) | (1<<6) | (1<<7) | (1<<8);
  buffer[2] = 0;
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<2) | (1<<6);
  buffer[1] = (1<<1) | (1<<3) | (1<<5) | (1<<7);
  buffer[2] = (1<<0) | (1<<8);
	_delay_ms(DELAY_LG);
  buffer[0] = 0;
  buffer[1] = (1<<0) | (1<<1) | (1<<2) | (1<<3)| (1<<5) | (1<<6) | (1<<7) | (1<<8);
  buffer[2] = 0;
	_delay_ms(DELAY_LG);
}


void modeRotationX(void){
  buffer[0] = (1<<0) | (1<<1) | (1<<2);
  buffer[1] = (1<<3) | (1<<4) | (1<<5);
  buffer[2] = (1<<6) | (1<<7) | (1<<8);
	_delay_ms(DELAY_LG);
  buffer[0] = 0;
  buffer[1] = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8);
  buffer[2] = 0;
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<6) | (1<<7) | (1<<8);
  buffer[1] = (1<<3) | (1<<4) | (1<<5);
  buffer[2] = (1<<0) | (1<<1) | (1<<2);
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<3) | (1<<4) | (1<<5);
  buffer[1] = (1<<3) | (1<<4) | (1<<5);
  buffer[2] = (1<<3) | (1<<4) | (1<<5);
	_delay_ms(DELAY_LG);
}

void modeRotationY(void){
  buffer[0] = (1<<0) | (1<<3) | (1<<6);
  buffer[1] = (1<<1) | (1<<4) | (1<<7);
  buffer[2] = (1<<2) | (1<<5) | (1<<8);
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<1) | (1<<4) | (1<<7);
  buffer[1] = (1<<1) | (1<<4) | (1<<7);
  buffer[2] = (1<<1) | (1<<4) | (1<<7);
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<2) | (1<<5) | (1<<8);
  buffer[1] = (1<<1) | (1<<4) | (1<<7);
  buffer[2] = (1<<0) | (1<<3) | (1<<6);
  _delay_ms(DELAY_LG);
  buffer[0] = 0;
  buffer[1] = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8);
  buffer[2] = 0;
	_delay_ms(DELAY_LG);
}

void modeRotationZ(void){
  buffer[0] = (1<<0) | (1<<4) | (1<<8);
  buffer[1] = (1<<0) | (1<<4) | (1<<8);
  buffer[2] = (1<<0) | (1<<4) | (1<<8);
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<1) | (1<<4) | (1<<7);
  buffer[1] = (1<<1) | (1<<4) | (1<<7);
  buffer[2] = (1<<1) | (1<<4) | (1<<7);
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<2) | (1<<4) | (1<<6);
  buffer[1] = (1<<2) | (1<<4) | (1<<6);
  buffer[2] = (1<<2) | (1<<4) | (1<<6);
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<3) | (1<<4) | (1<<5);
  buffer[1] = (1<<3) | (1<<4) | (1<<5);
  buffer[2] = (1<<3) | (1<<4) | (1<<5);
	_delay_ms(DELAY_LG);
}

void modeCherry(void){
  buffer[0] = 0;
  buffer[1] = (1<<4);
  buffer[2] = 0;
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<0) | (1<<2) | (1<<6) | (1<<8);
  buffer[1] = 0;
  buffer[2] = (1<<0) | (1<<2) | (1<<6) | (1<<8);
	_delay_ms(DELAY_LG);
  buffer[0] = 0;
  buffer[1] = (1<<4);
  buffer[2] = 0;
	_delay_ms(DELAY_LG);
  buffer[0] = (1<<4);
  buffer[1] = (1<<1) | (1<<3) | (1<<5) | (1<<7);
  buffer[2] = (1<<4);
	_delay_ms(DELAY_LG);
}


void cubeModes(uint8_t mode){
  uint8_t i;

  modeEnded = false;
  if (mode == 0) {
    for (i = 0; i < 2; i++)
      modeNibble();
  }
  else if (mode == 1) {
    for (i = 0; i < 4; i++)
      modeGuir();
  }
  else if (mode == 2) {
    for (i = 0; i < 4; i++)
      modeRotationX();
  }
  else if (mode == 3) {
    for (i = 0; i < 4; i++)
      modeRotationY();
  }
  else if (mode == 4) {
    for (i = 0; i < 4; i++)
      modeRotationZ();
  }
  else if (mode == 5) {
    for (i = 0; i < 4; i++)
      modeCherry();
  }

  modeEnded = true;
}

