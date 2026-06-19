#include "database.h"
#include "../storage/eeprom.h"
#include "../core/linked_list.h"
#include <stdlib.h>

void initDatabase(Node** head)
{
    *head = NULL;
    loadDatabase(head);
}

void saveDatabase(Node* head)
{
    extern int total_node;
    eeprom_save(head, total_node);
}

void loadDatabase(Node** head)
{
    extern int total_node;
    eeprom_load(head, &total_node);
}