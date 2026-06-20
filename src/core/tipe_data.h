#ifndef TIPE_DATA_H
#define TIPE_DATA_H

#include <stdint.h>

#define MAX_NODE 35 

typedef struct {
    char    id[6];             
    char    kode_nama;         
    char    kode_kategori;     
    int8_t  stok_tersedia;   
    int8_t  stok_dipinjam;   
    int8_t  stok_rusak;      
    char    kode_lokasi;       
    char    kode_status;       
    char    pemilik[8];        
    char    pic[8];          
} Inventaris;                

typedef struct Node {
    Inventaris data;
    struct Node* next;
} Node;

extern int total_node; 

#endif