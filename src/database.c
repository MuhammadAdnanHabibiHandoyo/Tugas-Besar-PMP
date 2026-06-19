#include "database.h"
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

int total_node = 0;

void simpanKeEEPROM(Node* head) {
    eeprom_write_word((uint16_t*)0, (uint16_t)total_node);
    int addr = 2; 
    Node* temp = head;
    while(temp != NULL) {
        eeprom_write_block(&(temp->data), (void*)addr, sizeof(Inventaris));
        addr += sizeof(Inventaris);
        temp = temp->next;
    }
}

void muatDariEEPROM(Node** head) {
    int saved_count = (int)eeprom_read_word((uint16_t*)0);
    if (saved_count <= 0 || saved_count > MAX_NODE) { total_node = 0; return; }
    
    int addr = 2; Node* tail = NULL;
    for (int i = 0; i < saved_count; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) break;
        
        eeprom_read_block(&(newNode->data), (const void*)addr, sizeof(Inventaris));
        newNode->next = NULL;
        
        if (*head == NULL) { *head = newNode; tail = newNode; } 
        else { tail->next = newNode; tail = newNode; }
        addr += sizeof(Inventaris);
    }
    total_node = saved_count;
}

void tambahData(Node** head, char* id, char* nama, char* kategori, int tersedia, int dipinjam, int rusak, char* lokasi, char* status, char* pemilik, char* pic) {
    if (total_node >= MAX_NODE) { printf_P(PSTR("[ERROR] EEPROM Hampir Penuh! (Maks %d)\n"), MAX_NODE); return; }
    Node* temp = *head;
    while(temp != NULL) {
        if (strcmp(temp->data.id, id) == 0) { printf_P(PSTR("[ERROR] ID '%s' sudah terdaftar!\n"), id); return; }
        temp = temp->next;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return;

    strcpy(newNode->data.id, id); strcpy(newNode->data.nama, nama); strcpy(newNode->data.kategori, kategori);
    newNode->data.stok_tersedia = tersedia; newNode->data.stok_dipinjam = dipinjam; newNode->data.stok_rusak = rusak;
    strcpy(newNode->data.lokasi, lokasi); 
    strcpy(newNode->data.status, status); 
    strcpy(newNode->data.pemilik, pemilik); strcpy(newNode->data.pic, pic);
    newNode->next = NULL;

    if (*head == NULL) *head = newNode;
    else { temp = *head; while (temp->next != NULL) temp = temp->next; temp->next = newNode; }
    total_node++; simpanKeEEPROM(*head); 
    printf_P(PSTR("[SUKSES] Data '%s' ditambahkan.\n"), nama);
}

void hapusData(Node** head, char* targetID) {
    if (*head == NULL) { printf_P(PSTR("[ERROR] Data kosong!\n")); return; }
    Node* temp = *head; Node* prev = NULL;
    
    if (strcmp(temp->data.id, targetID) == 0) {
        *head = temp->next; free(temp); total_node--; simpanKeEEPROM(*head); 
        printf_P(PSTR("[SUKSES] ID dihapus.\n")); return;
    }
    while(temp != NULL && strcmp(temp->data.id, targetID) != 0) { prev = temp; temp = temp->next; }
    if (temp == NULL) { printf_P(PSTR("[ERROR] ID Tidak Ditemukan\n")); return; }
    
    prev->next = temp->next; free(temp); total_node--; simpanKeEEPROM(*head); 
    printf_P(PSTR("[SUKSES] ID dihapus.\n"));
}

void cariData(Node* head, char* targetID) {
    Node* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            printf_P(PSTR("--- HASIL PENCARIAN ---\n"));
            printf_P(PSTR("ID: %s | Nama: %s | Kat: %s\n"), temp->data.id, temp->data.nama, temp->data.kategori);
            printf_P(PSTR("LOKASI: %s | Status Teks: %s\n"), temp->data.lokasi, temp->data.status);
            printf_P(PSTR("Pemilik: %s | PIC: %s\n"), temp->data.pemilik, temp->data.pic);
            printf_P(PSTR(">> TERSEDIA: %d | DIPINJAM: %d | RUSAK: %d\n"), temp->data.stok_tersedia, temp->data.stok_dipinjam, temp->data.stok_rusak);
            return;
        }
        temp = temp->next;
    }
    printf_P(PSTR("[ERROR] ID Tidak Ditemukan\n"));
}

void updateStokDetail(Node* head, char* targetID, char jenis, int perubahan) {
    Node* temp = head;
    while(temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            if (jenis == 'T' || jenis == 't') {
                if (temp->data.stok_tersedia + perubahan < 0) printf_P(PSTR("[ERROR] Stok Minus!\n"));
                else { temp->data.stok_tersedia += perubahan; printf_P(PSTR("[SUKSES] Tersedia: %d\n"), temp->data.stok_tersedia); }
            } else if (jenis == 'D' || jenis == 'd') {
                if (temp->data.stok_dipinjam + perubahan < 0) printf_P(PSTR("[ERROR] Stok Minus!\n"));
                else { temp->data.stok_dipinjam += perubahan; printf_P(PSTR("[SUKSES] Dipinjam: %d\n"), temp->data.stok_dipinjam); }
            } else if (jenis == 'R' || jenis == 'r') {
                if (temp->data.stok_rusak + perubahan < 0) printf_P(PSTR("[ERROR] Stok Minus!\n"));
                else { temp->data.stok_rusak += perubahan; printf_P(PSTR("[SUKSES] Rusak: %d\n"), temp->data.stok_rusak); }
            } else { printf_P(PSTR("[ERROR] Jenis tidak valid!\n")); return; }
            simpanKeEEPROM(head); return;
        }
        temp = temp->next;
    }
    printf_P(PSTR("[ERROR] ID Tidak Ditemukan\n"));
}

void updateStatus(Node* head, char* targetID, char* statusBaru) {
    Node* temp = head;
    while(temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            strcpy(temp->data.status, statusBaru);
            simpanKeEEPROM(head);
            printf_P(PSTR("[SUKSES] Status '%s' menjadi '%s'.\n"), temp->data.nama, statusBaru);
            return;
        }
        temp = temp->next;
    }
    printf_P(PSTR("[ERROR] ID Tidak Ditemukan\n"));
}