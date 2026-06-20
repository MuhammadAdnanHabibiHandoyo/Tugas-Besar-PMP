#include "ui.h"
#include "../database/database.h"
#include "../core/linked_list.h"
#include "../utils/decode.h"
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#ifndef E2END
#error "E2END NOT DEFINED - AVR HEADER NOT LOADED"
#endif

void tampilkanSemua(Node* head)
{
    if (head == NULL) { printf_P(PSTR("[INFO] Data kosong.\n")); return; }

    printf_P(PSTR("ID,Nama,Kategori,Tersedia,Dipinjam,Rusak,Lokasi,Status,Pemilik,PIC\n"));

    Node* temp = head;
    while (temp != NULL) {
        char nama[13], kategori[9], lokasi[7], status[10];

        decodeNama(temp->data.kode_nama, nama);
        decodeKategori(temp->data.kode_kategori, kategori);
        decodeLokasi(temp->data.kode_lokasi, lokasi);
        decodeStatus(temp->data.kode_status, status);

        printf_P(PSTR("%s,%s,%s,%d,%d,%d,%s,%s,%s,%s\n"),
                 temp->data.id, nama, kategori,
                 temp->data.stok_tersedia,
                 temp->data.stok_dipinjam,
                 temp->data.stok_rusak,
                 lokasi, status,
                 temp->data.pemilik, temp->data.pic);

        temp = temp->next;
    }
}

void tampilkanRingkasan(Node* head)
{
    int t_item = 0, d_item = 0, r_item = 0, habis = 0;
    Node* temp = head;
    while (temp != NULL) {
        t_item += temp->data.stok_tersedia;
        d_item += temp->data.stok_dipinjam;
        r_item += temp->data.stok_rusak;
        if (temp->data.stok_tersedia == 0 &&
            temp->data.stok_dipinjam == 0 &&
            temp->data.stok_rusak    == 0) habis++;
        temp = temp->next;
    }

    printf_P(PSTR("--- RINGKASAN INVENTARIS LAB ---\n"));
    printf_P(PSTR("Total Slot Terpakai : %d / %d\n"), total_node, MAX_NODE);

    // TAMBAHAN: indikator kapasitas
    if (total_node >= MAX_NODE) {
        printf_P(PSTR("[!!] DATABASE PENUH - Hapus data dulu\n"));
    } else if (total_node >= MAX_NODE - 5) {
        printf_P(PSTR("[!]  Sisa slot: %d lagi\n"), MAX_NODE - total_node);
    }

    printf_P(PSTR("Total Item Tersedia : %d unit\n"), t_item);
    printf_P(PSTR("Total Item Dipinjam : %d unit\n"), d_item);
    printf_P(PSTR("Total Item Rusak    : %d unit\n"), r_item);
    printf_P(PSTR("Jenis Barang Habis  : %d jenis\n"), habis);
}

void cetakPanduan(void)
{
    printf_P(PSTR("=== PANDUAN MENU INVENTARIS ===\n"));
    printf_P(PSTR("Ketik angka & data sesuai format, lalu Enter\n"));
    printf_P(PSTR("[1] Tambah Data : 1,ID,Sandi(AM5AT),Pemilik,PIC\n"));
    printf_P(PSTR("[2] Cari Data   : 2,ID\n"));
    printf_P(PSTR("[3] Hapus Data  : 3,ID\n"));
    printf_P(PSTR("[4] Update Stok : 4,ID,Jenis(T/D/R),Angka Ubah\n"));
    printf_P(PSTR("[5] Edit Status : 5,ID,Kode(T/D/R)\n"));
    printf_P(PSTR("[6] Cetak Tabel : 6\n"));
    printf_P(PSTR("[7] Panduan     : 7\n"));
    printf_P(PSTR("[8] Ringkasan   : 8\n"));
    printf_P(PSTR("[9] Info Memori : 9\n"));
    printf_P(PSTR("===============================\n"));
}

void infoMemori(void)
{
    int eeprom_size    = E2END + 1;
    int eeprom_terpakai = 2 + (total_node * (int)sizeof(Inventaris));
    int eeprom_sisa    = eeprom_size - eeprom_terpakai;

    printf_P(PSTR("=== INFO KESEHATAN MEMORI ===\n"));
    printf_P(PSTR("Kapasitas Total EEPROM : %d byte\n"), eeprom_size);
    printf_P(PSTR("Ukuran 1 Data (Struct) : %u byte\n"), (unsigned int)sizeof(Inventaris));
    printf_P(PSTR("Ukuran 1 Node (RAM)    : %u byte\n"), (unsigned int)sizeof(Node));
    printf_P(PSTR("-----------------------------\n"));
    printf_P(PSTR("Jumlah Data Tersimpan  : %d dari %d Maksimal\n"), total_node, MAX_NODE);
    printf_P(PSTR("Total EEPROM Terpakai  : %d byte\n"), eeprom_terpakai);
    printf_P(PSTR("Sisa Memori EEPROM     : %d byte\n"), eeprom_sisa);
    printf_P(PSTR("=============================\n"));
}

void jalankanMenu(void)
{
    Node* head = NULL;
    loadDatabase(&head);
    char buffer[150];

    cetakPanduan();

    while (1) {
        int idx = 0; int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c != '\r' && idx < 149) buffer[idx++] = c;
        }
        buffer[idx] = '\0';
        if (idx == 0) continue;

        if (buffer[0] < '0' || buffer[0] > '9') continue;

        int pilihan = -1;
        char inputID[6] = {0};
        sscanf(buffer, "%d", &pilihan);

        if (pilihan == 1) {
            char inputKode[20]={0}, pemilik[8]={0}, pic[8]={0};
            int p, stok = 0;
            char kNama, kKat, kLok, kStat;

            if (sscanf(buffer, "%d,%5[^,],%19[^,],%7[^,],%7[^,]",
                       &p, inputID, inputKode, pemilik, pic) == 5) {
                if (sscanf(inputKode, "%c%c%d%c%c",
                           &kNama, &kKat, &stok, &kLok, &kStat) >= 4) {
                    int8_t t=0, d=0, r=0;
                    if      (kStat == 'D') d = (int8_t)stok;
                    else if (kStat == 'R') r = (int8_t)stok;
                    else                   t = (int8_t)stok;

                    tambahData(&head, inputID,
                               kNama, kKat,
                               t, d, r,
                               kLok, kStat,
                               pemilik, pic);
                    saveDatabase(head);
                }
            }
        }
        else if (pilihan == 2) {
            int p;
            if (sscanf(buffer, "%d,%5[^,]", &p, inputID) == 2) {
                cariData(head, inputID);
            }
        }
        else if (pilihan == 3) {
            int p;
            if (sscanf(buffer, "%d,%5[^,]", &p, inputID) == 2) {
                hapusData(&head, inputID);
                saveDatabase(head);
            }
        }
        else if (pilihan == 4) {
            int p, ubah; char jenis;
            if (sscanf(buffer, "%d,%5[^,],%c,%d", &p, inputID, &jenis, &ubah) == 4) {
                updateStokDetail(head, inputID, jenis, ubah);
                saveDatabase(head);
            }
        }
        else if (pilihan == 5) {
            // DIFIX: terima 1 karakter kode (T/D/R), sesuai updateStatus yang baru
            int p; char kStatBaru;
            if (sscanf(buffer, "%d,%5[^,],%c", &p, inputID, &kStatBaru) == 3) {
                updateStatus(head, inputID, kStatBaru);
                saveDatabase(head);
            }
        }
        else if (pilihan == 6) { tampilkanSemua(head); }
        else if (pilihan == 7) { cetakPanduan(); }
        else if (pilihan == 8) { tampilkanRingkasan(head); }
        else if (pilihan == 9) { infoMemori(); }
    }
}