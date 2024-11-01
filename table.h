#ifndef TABLE_H
#define TABLE_H

#include "btree.h"
#include "pager.h"
#include "serializers.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  uint32_t root_page_num;
  Pager *pager;
} Table;

Table *open_db(void);
void close_db(Table *);
void *row_slot(Pager *, uint32_t);

#endif // !TABLE_H
