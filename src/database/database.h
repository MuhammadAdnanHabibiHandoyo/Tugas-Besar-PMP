#ifndef DATABASE_H
#define DATABASE_H

#include "../core/linked_list.h"

void initDatabase(Node** head);
void saveDatabase(Node* head);
void loadDatabase(Node** head);

#endif