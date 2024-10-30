#include "table.h"

Table *open_db() {
  Table *table = (Table *)malloc(sizeof(Table));
  Pager *pager = open_pager();
  table->num_rows = pager->file_length / ROW_SIZE;
  table->pager = pager;
  return table;
}

void close_db(Table *table) {
  Pager *pager = table->pager;
  uint32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;

  for (int page_num = 0; page_num < num_full_pages; page_num++) {
    void *page = pager->pages[page_num];
    if (page == NULL) {
      continue;
    }
    flush_page(pager, page_num, PAGE_SIZE);
    free(page);
    pager->pages[page_num] = NULL;
  }

  uint32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = num_full_pages;
    if (pager->pages[page_num] != NULL) {
      flush_page(pager, page_num, num_additional_rows * ROW_SIZE);
      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }

  close(pager->file_descriptor);
  free(pager);
  free(table);
}
