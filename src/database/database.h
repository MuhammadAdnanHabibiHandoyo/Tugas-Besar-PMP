#ifndef DATABASE_H
#define DATABASE_H

#include "../core/linked_list.h"

#ifdef __cplusplus
extern "C" {
#endif

void initDatabase(Node** head);
void saveDatabase(Node* head);
void loadDatabase(Node** head);

#ifdef __cplusplus
}
#endif

#endif