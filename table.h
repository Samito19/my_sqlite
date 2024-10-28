#ifndef TABLE_H
#define TABLE_H

#include "serializers.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  void *pages[MAX_PAGES];
  uint32_t num_rows;
} Table;

Table *new_table(void);
void free_table(Table *);
void *row_slot(Table *, uint32_t);

#endif // !TABLE_H
