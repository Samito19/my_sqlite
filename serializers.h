#ifndef SERIALIZERS_H
#define SERIALIZERS_H

#include "constants.h"
#include <stdint.h>
#include <string.h>

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

const static uint32_t ID_SIZE = size_of_attribute(Row, id);
const static uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const static uint32_t EMAIL_SIZE = size_of_attribute(Row, email);

const static uint32_t ID_OFFSET = 0;
const static uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const static uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const static uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const static uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const static uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * MAX_PAGES;

void serialize(void *, Row *);
void deserialize(Row *, void *);

#endif // !SERIALIZERS_H
