#ifndef CURSOR_H
#define CURSOR_H

#include "table.h"
#include <stdbool.h>

typedef struct {
  Table *table;
  uint32_t cell_num;
  uint32_t page_num;
  bool end_of_table;
} Cursor;

Cursor *table_start(Table *);
Cursor *table_find(Table *, uint32_t);
void cursor_advance(Cursor *);
void *cursor_value(Cursor *);
Cursor *leaf_node_find(Table *, uint32_t, uint32_t);
void leaf_node_split_insert(Cursor *, uint32_t, Row *);
Cursor *internal_node_find(Table *, uint32_t, uint32_t);

#endif // !CURSOR_H
#define CURSOR_H
