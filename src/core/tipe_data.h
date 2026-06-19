#ifndef TIPE_DATA_H
#define TIPE_DATA_H

#include <stdint.h>

#define MAX_NODE 13 

typedef struct {
    char id[8];          
    char nama[15];       
    char kategori[12];   
    int8_t stok_tersedia;
    int8_t stok_dipinjam;
    int8_t stok_rusak;   
    char lokasi[6];      
    char status[10];     
    char pemilik[10];    
    char pic[10];        
} Inventaris;

typedef struct Node {
    Inventaris data;
    struct Node* next;
} Node;

extern int total_node; 

#endif