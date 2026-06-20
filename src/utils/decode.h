#ifndef DECODE_H
#define DECODE_H

#ifdef __cplusplus
extern "C" {
#endif

void decodeNama(char kode, char* out);
void decodeKategori(char kode, char* out);
void decodeLokasi(char kode, char* out);
void decodeStatus(char kode, char* out);

#ifdef __cplusplus
}
#endif

#endif