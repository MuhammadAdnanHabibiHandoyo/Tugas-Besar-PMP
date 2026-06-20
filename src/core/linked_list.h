#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "tipe_data.h"
#include <stdint.h>

extern int total_node;

void tambahData(Node** head, char* id, char kNama, char kKat,
                int8_t t, int8_t d, int8_t r,
                char kLok, char kStat,
                char* pemilik, char* pic);

void hapusData(Node** head, char* targetID);
void cariData(Node* head, char* targetID);

void updateStokDetail(Node* head, char* targetID, char jenis, int perubahan);
void updateStatus(Node* head, char* targetID, char kStatBaru);

#endif