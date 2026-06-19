#ifndef DATABASE_H
#define DATABASE_H

#include "tipe_data.h"

void muatDariEEPROM(Node** head);
void simpanKeEEPROM(Node* head);
void tambahData(Node** head, char* id, char* nama, char* kategori, int tersedia, int dipinjam, int rusak, char* lokasi, char* status, char* pemilik, char* pic);
void hapusData(Node** head, char* targetID);
void cariData(Node* head, char* targetID);
void updateStokDetail(Node* head, char* targetID, char jenis, int perubahan);
void updateStatus(Node* head, char* targetID, char* statusBaru);

#endif