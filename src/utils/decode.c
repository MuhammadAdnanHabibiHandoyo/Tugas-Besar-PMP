#include "decode.h"
#include <string.h>
#include <avr/pgmspace.h>

void decodeNama(char kode, char* out) {
    switch(kode) {
        case 'A': strcpy_P(out, PSTR("Arduino Uno")); break;
        case 'M': strcpy_P(out, PSTR("Multimeter"));  break;
        case 'S': strcpy_P(out, PSTR("Sensor Suhu")); break;
        case 'R': strcpy_P(out, PSTR("Resistor"));    break;
        case 'C': strcpy_P(out, PSTR("Kapasitor"));   break;
        case 'O': strcpy_P(out, PSTR("Osiloskop"));   break;
        default:  strcpy_P(out, PSTR("?"));           break;
    }
}

void decodeKategori(char kode, char* out) {
    switch(kode) {
        case 'M': strcpy_P(out, PSTR("Mikro"));     break;
        case 'U': strcpy_P(out, PSTR("Alat Ukur")); break;
        case 'S': strcpy_P(out, PSTR("Sensor"));    break;
        case 'K': strcpy_P(out, PSTR("Komponen"));  break;
        default:  strcpy_P(out, PSTR("?"));         break;
    }
}

void decodeLokasi(char kode, char* out) {
    switch(kode) {
        case 'A': strcpy_P(out, PSTR("Rak A")); break;
        case 'B': strcpy_P(out, PSTR("Rak B")); break;
        case 'C': strcpy_P(out, PSTR("Rak C")); break;
        default:  strcpy_P(out, PSTR("?"));     break;
    }
}

void decodeStatus(char kode, char* out) {
    switch(kode) {
        case 'T': strcpy_P(out, PSTR("Tersedia")); break;
        case 'D': strcpy_P(out, PSTR("Dipinjam")); break;
        case 'R': strcpy_P(out, PSTR("Rusak"));    break;
        default:  strcpy_P(out, PSTR("?"));        break;
    }
}