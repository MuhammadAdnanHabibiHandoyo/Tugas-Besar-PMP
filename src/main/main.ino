#include <Arduino.h>
#include "../ui/ui.h"
#include "database.h"
#include "tipe_data.h"
extern "C" {
    #include "../ui/ui.h"
}

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
    fdevopen(&serial_putc, &serial_getc); 
    delay(1000); 

    jalankanMenu(); 
    
    return 0;
}