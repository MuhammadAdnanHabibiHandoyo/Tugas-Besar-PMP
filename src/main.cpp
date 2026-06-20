#include <Arduino.h>
#include <avr/eeprom.h>
#include "ui/ui.h"
#include "database/database.h"
#include "core/tipe_data.h"


int serial_putc(char c, FILE *) {
    Serial.write(c);
    return c;
}

int serial_getc(FILE *) {
    while (!Serial.available()); 
    return Serial.read();
}

int main(void) {
    init(); 
    Serial.begin(9600);
    //for (int i = 0; i < 1024; i++)
    //{
    //    eeprom_write_byte((uint8_t*)i, 0xFF);
    //}

    fdevopen(&serial_putc, &serial_getc); 
    delay(1000); 

    jalankanMenu(); 
    
    return 0;
}