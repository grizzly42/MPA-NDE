#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PD3  // PD3 odpovídá fyzickému pinu 5

int main(void) {
    DDRD |= (1 << LED_PIN); // Nastaví PD3 jako výstup

    while (1) {
        PORTD ^= (1 << LED_PIN); // Přepne stav LED
        _delay_ms(500);          // Počká 500 ms
    }

    return 0;
}