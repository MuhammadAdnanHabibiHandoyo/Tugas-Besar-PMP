#include "eeprom.h"
#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include "../core/linked_list.h"
/* =========================================================
   EEPROM FORMAT:
   [0-1]   -> total_node (2 bytes)
   [2..]   -> data inventaris
   ========================================================= */

#define EEPROM_ADDR_DATA_START 2


/* =========================================================
   SAVE LINKED LIST TO EEPROM
   ========================================================= */

void eeprom_save(Node* head, int total_node)
{
    // simpan jumlah data
    eeprom_write_word((uint16_t*)0, (uint16_t)total_node);

    int addr = EEPROM_ADDR_DATA_START;
    Node* temp = head;

    while (temp != NULL)
    {
        eeprom_write_block(
            &(temp->data),
            (void*)addr,
            sizeof(Inventaris)
        );

        addr += sizeof(Inventaris);
        temp = temp->next;
    }
}


/* =========================================================
   LOAD EEPROM TO LINKED LIST
   ========================================================= */

void eeprom_load(Node** head, int* total_node)
{
    int saved_count = (int)eeprom_read_word((uint16_t*)0);

    if (saved_count <= 0 || saved_count > MAX_NODE)
    {
        *total_node = 0;
        return;
    }

    int addr = EEPROM_ADDR_DATA_START;
    Node* tail = NULL;

    for (int i = 0; i < saved_count; i++)
    {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (!newNode) break;

        eeprom_read_block(
            &(newNode->data),
            (const void*)addr,
            sizeof(Inventaris)
        );

        newNode->next = NULL;

        if (*head == NULL)
        {
            *head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        addr += sizeof(Inventaris);
    }

    *total_node = saved_count;
}