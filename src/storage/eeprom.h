#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <stdint.h>
#include "../core/linked_list.h"

#define EEPROM_BASE_ADDR 2

void eeprom_save(Node* head, int total_node);
void eeprom_load(Node** head, int* total_node);

#endif