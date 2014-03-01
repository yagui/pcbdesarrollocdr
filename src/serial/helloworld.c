#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"


int main(void){
    DDRC |= (1<<PC3);

    USART0Setup(USART_BAUDRATE_115200,USART_CHARSIZE_8BIT,USART_PARITY_DIS,USART_STOP_1BIT,USART_MODE_ASYNC);

    while(1){
        USART0PutsP(PSTR("Hello PC!!!!\n"));
        _delay_ms(1000);
        PORTC |= (1<<PC3);
        _delay_ms(1000);
        PORTC &= ~(1<<PC3);
    }
}
