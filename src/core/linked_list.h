#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "tipe_data.h"

// CRUD OPERATIONS
void tambahData(Node** head, char* id, char* nama, char* kategori, int tersedia, int dipinjam, int rusak, char* lokasi, char* status, char* pemilik, char* pic);

void hapusData(Node** head, char* targetID);
void cariData(Node* head, char* targetID);

void updateStokDetail(Node* head, char* targetID, char jenis, int perubahan);

void updateStatus(Node* head, char* targetID, char* statusBaru);

#endif