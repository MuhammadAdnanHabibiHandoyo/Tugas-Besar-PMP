#include "ui.h"
#include "../database/database.h"
#include "../core/linked_list.h"
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>


#ifndef E2END
#error "E2END NOT DEFINED - AVR HEADER NOT LOADED"
#endif

void tampilkanSemua(Node* head) {
    if (head == NULL) { printf_P(PSTR("[INFO] Data kosong.\n")); return; }
    printf_P(PSTR("ID,Nama Komponen,Kategori,Tersedia,Dipinjam,Rusak,Lokasi,Status,Pemilik,PIC\n"));
    Node* temp = head;
    while(temp != NULL) {
        printf_P(PSTR("%s,%s,%s,%d,%d,%d,%s,%s,%s,%s\n"), 
               temp->data.id, temp->data.nama, temp->data.kategori, 
               temp->data.stok_tersedia, temp->data.stok_dipinjam, temp->data.stok_rusak, 
               temp->data.lokasi, temp->data.status, temp->data.pemilik, temp->data.pic);
        temp = temp->next;
    }
}

void tampilkanRingkasan(Node* head) {
    int t_item = 0, d_item = 0, r_item = 0, habis = 0;
    Node* temp = head;
    while (temp != NULL) {
        t_item += temp->data.stok_tersedia;
        d_item += temp->data.stok_dipinjam;
        r_item += temp->data.stok_rusak;
        if (temp->data.stok_tersedia == 0 && temp->data.stok_dipinjam == 0 && temp->data.stok_rusak == 0) habis++;
        temp = temp->next;
    }
    printf_P(PSTR("--- RINGKASAN INVENTARIS LAB ---\n"));
    printf_P(PSTR("Total Slot Terpakai : %d / %d\n"), total_node, MAX_NODE);
    printf_P(PSTR("Total Item Tersedia : %d unit\n"), t_item);
    printf_P(PSTR("Total Item Dipinjam : %d unit\n"), d_item);
    printf_P(PSTR("Total Item Rusak    : %d unit\n"), r_item);
    printf_P(PSTR("Jenis Barang Habis  : %d jenis\n"), habis);
}

void cetakPanduan(void) {
    printf_P(PSTR("=== PANDUAN MENU INVENTARIS ===\n"));
    printf_P(PSTR("Ketik angka & data sesuai format, lalu Enter\n"));
    printf_P(PSTR("[1] Tambah Data: 1,ID,Sandi(AM5AT),Pemilik,PIC\n"));
    printf_P(PSTR("[2] Cari Data  : 2,ID \n"));
    printf_P(PSTR("[3] Hapus Data : 3,ID \n"));
    printf_P(PSTR("[4] Update Stok: 4,ID,Jenis(T/D/R),Angka Ubah \n"));
    printf_P(PSTR("[5] Edit Status: 5,ID,Status Teks Baru \n"));
    printf_P(PSTR("[6] Cetak Tabel: 6 \n"));
    printf_P(PSTR("[7] Tampilkan Panduan Ini Lagi: 7 \n"));
    printf_P(PSTR("[8] Cetak Ringkasan Database  : 8 \n"));
    printf_P(PSTR("[9] Cek Kapasitas Memori EEPROM : 9 \n"));
    printf_P(PSTR("===============================\n"));
}

void infoMemori(void) {
    int eeprom_size = E2END + 1;
    // Kalkulasi: 2 byte untuk simpan jumlah data + (jumlah data * ukuran 1 struct Inventaris)
    int eeprom_terpakai = 2 + (total_node * sizeof(Inventaris));
    int eeprom_sisa = eeprom_size - eeprom_terpakai;

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

void jalankanMenu(void) {
    Node* head = NULL;
    loadDatabase(&head);
    char buffer[150]; 

    cetakPanduan();

    while(1) {
        int idx = 0; int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c != '\r' && idx < 149) buffer[idx++] = c;
        }
        buffer[idx] = '\0';
        if (idx == 0) continue; 

        int pilihan = -1; char inputID[8] = {0}; 
        sscanf(buffer, "%d", &pilihan);

        if (pilihan == 1) {
            char inputKode[20]={0}; char pemilik[10]={0}; char pic[10]={0};       
            int p, stok = 0; char kNama, kKat, kLok, kStat;
            
            if (sscanf(buffer, "%d,%7[^,],%19[^,],%9[^,],%9[^,]", &p, inputID, inputKode, pemilik, pic) == 5) {
                if (sscanf(inputKode, "%c%c%d%c%c", &kNama, &kKat, &stok, &kLok, &kStat) >= 4) {
                    char nama[15]="?", kategori[12]="?", lokasi[6]="?";
                    char status_awal[10]="?"; 
                    
                    if (kNama == 'A') strcpy(nama, "Arduino Uno"); else if (kNama == 'M') strcpy(nama, "Multimeter");
                    else if (kNama == 'S') strcpy(nama, "Sensor Suhu"); else if (kNama == 'R') strcpy(nama, "Resistor");
                    else if (kNama == 'C') strcpy(nama, "Kapasitor"); else if (kNama == 'O') strcpy(nama, "Osiloskop");

                    if (kKat == 'M') strcpy(kategori, "Mikro"); else if (kKat == 'U') strcpy(kategori, "Alat Ukur");
                    else if (kKat == 'S') strcpy(kategori, "Sensor"); else if (kKat == 'K') strcpy(kategori, "Komponen");

                    if (kLok == 'A') strcpy(lokasi, "Rak A"); else if (kLok == 'B') strcpy(lokasi, "Rak B");
                    else if (kLok == 'C') strcpy(lokasi, "Rak C");

                    int t = 0, d = 0, r = 0;
                    if (kStat == 'D') { d = stok; strcpy(status_awal, "Dipinjam"); }
                    else if (kStat == 'R') { r = stok; strcpy(status_awal, "Rusak"); }
                    else { t = stok; strcpy(status_awal, "Tersedia"); }

                    tambahData(&head, inputID, nama, kategori, t, d, r, lokasi, status_awal, pemilik, pic);
                    saveDatabase(head);
                }
            } 
            // Silent Fail: Tidak memunculkan error format di Excel saat mengetik data
        }
        else if (pilihan == 2) {
            int p; if (sscanf(buffer, "%d,%7[^,]", &p, inputID) == 2) {
                cariData(head, inputID);
                saveDatabase(head);
            }
        } 
        else if (pilihan == 3) {
            int p; if (sscanf(buffer, "%d,%7[^,]", &p, inputID) == 2){
                hapusData(&head, inputID);
                saveDatabase(head);
            }
        } 
        else if (pilihan == 4) {
            int p, ubah; char jenis;
            if (sscanf(buffer, "%d,%7[^,],%c,%d", &p, inputID, &jenis, &ubah) == 4) {
                updateStokDetail(head, inputID, jenis, ubah);
                saveDatabase(head);
            }
        }
        else if (pilihan == 5) {
            int p; char statBaru[10]={0};
            if (sscanf(buffer, "%d,%7[^,],%9[^,]", &p, inputID, statBaru) == 3){
                updateStatus(head, inputID, statBaru);
                saveDatabase(head);
            }
        }
        else if (pilihan == 6) {
            tampilkanSemua(head); 
        }
        else if (pilihan == 7) {
            cetakPanduan(); 
        }
        else if (pilihan == 8) {
            tampilkanRingkasan(head);
        }
        else if (pilihan == 9) {
            infoMemori();
        }
    }
}