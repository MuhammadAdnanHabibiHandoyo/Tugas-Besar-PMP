#include "linked_list.h"
#include "../utils/decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

int total_node = 0;

void tambahData(Node** head, char* id, char kNama, char kKat,
                int8_t t, int8_t d, int8_t r,
                char kLok, char kStat,
                char* pemilik, char* pic)
{
    if (total_node >= MAX_NODE) {
        printf_P(PSTR("[PENUH] Slot penuh! Maksimal %d data.\n"), MAX_NODE);
        return;
    }

    Node* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->data.id, id) == 0) return;
        temp = temp->next;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf_P(PSTR("Memori Udah Habis\n"));
        return;
    }

    memset(newNode, 0, sizeof(Node));

    strncpy(newNode->data.id, id, sizeof(newNode->data.id) - 1);
    newNode->data.kode_nama     = kNama;
    newNode->data.kode_kategori = kKat;
    newNode->data.stok_tersedia = t;
    newNode->data.stok_dipinjam = d;
    newNode->data.stok_rusak    = r;
    newNode->data.kode_lokasi   = kLok;
    newNode->data.kode_status   = kStat;
    strncpy(newNode->data.pemilik, pemilik, sizeof(newNode->data.pemilik) - 1);
    strncpy(newNode->data.pic,     pic,     sizeof(newNode->data.pic) - 1);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    total_node++;
}

void hapusData(Node** head, char* targetID)
{
    if (*head == NULL) return;

    Node* temp = *head;
    Node* prev = NULL;

    if (strcmp(temp->data.id, targetID) == 0) {
        Node* del = temp;
        *head = temp->next;
        free(del);
        total_node--;
        return;
    }

    while (temp != NULL && strcmp(temp->data.id, targetID) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) return;

    prev->next = temp->next;
    free(temp);
    total_node--;
}

void cariData(Node* head, char* targetID)
{
    Node* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            char nama[13], kategori[9], lokasi[7], status[10];

            decodeNama(temp->data.kode_nama, nama);
            decodeKategori(temp->data.kode_kategori, kategori);
            decodeLokasi(temp->data.kode_lokasi, lokasi);
            decodeStatus(temp->data.kode_status, status);

            printf_P(PSTR("ID       : %s\n"), temp->data.id);
            printf_P(PSTR("Nama     : %s\n"), nama);
            printf_P(PSTR("Kategori : %s\n"), kategori);
            printf_P(PSTR("Lokasi   : %s\n"), lokasi);
            printf_P(PSTR("Status   : %s\n"), status);
            printf_P(PSTR("Tersedia : %d\n"), temp->data.stok_tersedia);
            printf_P(PSTR("Dipinjam : %d\n"), temp->data.stok_dipinjam);
            printf_P(PSTR("Rusak    : %d\n"), temp->data.stok_rusak);
            printf_P(PSTR("Pemilik  : %s\n"), temp->data.pemilik);
            printf_P(PSTR("PIC      : %s\n"), temp->data.pic);
            return;
        }
        temp = temp->next;
    }

    printf_P(PSTR("ID %s tidak ditemukan\n"), targetID);
}

void updateStokDetail(Node* head, char* targetID, char jenis, int perubahan)
{
    Node* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            if (jenis == 'T' && temp->data.stok_tersedia + perubahan < 0) {
                printf_P(PSTR("[ERROR] Stok Minus!\n"));
                return;
            }
            if      (jenis == 'T') temp->data.stok_tersedia += perubahan;
            else if (jenis == 'D') temp->data.stok_dipinjam += perubahan;
            else if (jenis == 'R') temp->data.stok_rusak    += perubahan;
            return;
        }
        temp = temp->next;
    }
}

void updateStatus(Node* head, char* targetID, char kStatBaru)
{
    Node* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            temp->data.kode_status = kStatBaru;
            return;
        }
        temp = temp->next;
    }
}