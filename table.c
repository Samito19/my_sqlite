#include "table.h"

Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->num_rows = 0;
  for (int page_index = 0; page_index < MAX_PAGES; page_index++) {
    table->pages[page_index] = NULL;
  }
  return table;
}

void free_table(Table *table) {
  for (int page_index = 0; page_index < MAX_PAGES; page_index++) {
    free(table->pages[page_index]);
  }
  free(table);
}

void *row_slot(Table *table, uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void *page = table->pages[page_num];
  if (page == NULL) {
    page = table->pages[page_num] = malloc(PAGE_SIZE);
  }

  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t bytes_offset = row_offset * ROW_SIZE;

  return page + bytes_offset;
}
