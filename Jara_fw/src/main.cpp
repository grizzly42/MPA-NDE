#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// Definice pinů
#define END_SWITCH1_PIN PD6
#define END_SWITCH2_PIN PC2
#define BURN_PIN1      PC3
#define BURN_PIN2      PB0
#define BUTTON_PIN     PD4
#define LED_PIN        PD3  // LED signalizace

bool antennaDeployed = false;

// UART inicializace (9600 baud, 8N1)
void uart_init(void) {
    uint16_t ubrr = 103;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putchar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_print(const char *s) {
    while (*s) {
        uart_putchar(*s++);
    }
}

void setup() {
    // Vstupy
    DDRD &= ~(1 << END_SWITCH1_PIN);
    DDRC &= ~(1 << END_SWITCH2_PIN);
    DDRD &= ~(1 << BUTTON_PIN);

    // Pull-up rezistory
    PORTD |= (1 << END_SWITCH1_PIN);
    PORTC |= (1 << END_SWITCH2_PIN);
    PORTD |= (1 << BUTTON_PIN);

    // Výstupy
    DDRC |= (1 << BURN_PIN1);
    DDRB |= (1 << BURN_PIN2);
    DDRD |= (1 << LED_PIN);

    PORTC &= ~(1 << BURN_PIN1);
    PORTB &= ~(1 << BURN_PIN2);
    PORTD &= ~(1 << LED_PIN);

    uart_init();
    uart_print("System initialized\r\n");
}

bool isEndSwitchPressed() {
    return !(PIND & (1 << END_SWITCH1_PIN)) && !(PINC & (1 << END_SWITCH2_PIN));
}

bool isButtonPressed() {
    return !(PIND & (1 << BUTTON_PIN));
}

void burnWire() {
    uart_print("Burning wire...\r\n");
    PORTC |= (1 << BURN_PIN1);
    PORTB |= (1 << BURN_PIN2);
    _delay_ms(1000);
    PORTC &= ~(1 << BURN_PIN1);
    PORTB &= ~(1 << BURN_PIN2);
    uart_print("Burn complete\r\n");
}

int main(void) {
    setup();

    while (1) {
        if (isEndSwitchPressed() && !antennaDeployed) {
            uart_print("Antenna ready for deployment\r\n");

            /*
            uart_print("Waiting for button press...\r\n");
            while (!isButtonPressed()) {
                // čekáme na stisk tlačítka
            }
            uart_print("Button pressed, starting countdown...\r\n");
            */

            uart_print("Countdown started\r\n");
            for (int i = 0; i < 100; i++) {
                _delay_ms(100);
            }

            burnWire();
            antennaDeployed = true;

            uart_print("posílám data\r\n");
            PORTD |= (1 << LED_PIN); // Zapni LED signalizaci
        }

        if (antennaDeployed) {
            // případná další logika
        }
    }
}
