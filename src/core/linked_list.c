#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int total_node = 0;

//Tambah Data
void tambahData(Node** head, char* id, char* nama, char* kategori, int tersedia, int dipinjam, int rusak, char* lokasi, char* status, char* pemilik, char* pic)
{
    if (total_node >= MAX_NODE) return;

    Node* temp = *head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, id) == 0) return;
        temp = temp->next;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;

    strcpy(newNode->data.id, id);
    strcpy(newNode->data.nama, nama);
    strcpy(newNode->data.kategori, kategori);

    newNode->data.stok_tersedia = tersedia;
    newNode->data.stok_dipinjam = dipinjam;
    newNode->data.stok_rusak = rusak;

    strcpy(newNode->data.lokasi, lokasi);
    strcpy(newNode->data.status, status);
    strcpy(newNode->data.pemilik, pemilik);
    strcpy(newNode->data.pic, pic);

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

//Hapus Data
void hapusData(Node** head, char* targetID)
{
    if (*head == NULL) return;

    Node* temp = *head;
    Node* prev = NULL;

    if (strcmp(temp->data.id, targetID) == 0) {
        *head = temp->next;
        free(temp);
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

//Cari Data
void cariData(Node* head, char* targetID)
{
    Node* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            return;
        }
        temp = temp->next;
    }
}

//Update Stok
void updateStokDetail(Node* head,
                      char* targetID,
                      char jenis,
                      int perubahan)
{
    Node* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {

            if (jenis == 'T')
                temp->data.stok_tersedia += perubahan;
            else if (jenis == 'D')
                temp->data.stok_dipinjam += perubahan;
            else if (jenis == 'R')
                temp->data.stok_rusak += perubahan;

            return;
        }
        temp = temp->next;
    }
}

//Update Status
void updateStatus(Node* head, char* targetID, char* statusBaru)
{
    Node* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->data.id, targetID) == 0) {
            strcpy(temp->data.status, statusBaru);
            return;
        }
        temp = temp->next;
    }
}