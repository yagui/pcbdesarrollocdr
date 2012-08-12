#include <avr/io.h>
#include <util/delay.h>

#include "board.h"

int main (void) {
  int i;
    // Iniciamos y apagamos los LEDs
  Led1Init();
  
  while(1){
    Led1Toggle();
    for(i=0;i<50000;i++);
  }

}
