#include "cursor.h"

Cursor *table_start(Table *table) {
  Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = 0;
  cursor->end_of_table = (table->num_rows == 0);

  return cursor;
}

Cursor *table_end(Table *table) {
  Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = table->num_rows;
  cursor->end_of_table = true;

  return cursor;
}

void cursor_advance(Cursor *cursor) {
  cursor->row_num += 1;
  cursor->end_of_table = cursor->row_num == cursor->table->num_rows;
}

void *cursor_value(Cursor *cursor) {
  uint32_t page_num = cursor->row_num / ROWS_PER_PAGE;
  void *page = get_page(cursor->table->pager, page_num);
  uint32_t row_offset = cursor->row_num % ROWS_PER_PAGE;
  uint32_t bytes_offset = row_offset * ROW_SIZE;
  return page + bytes_offset;
}

// void *row_slot(Pager *pager, uint32_t row_num) {
//   uint32_t page_num = row_num / ROWS_PER_PAGE;
//   void *page = get_page(pager, page_num);
//   uint32_t row_offset = row_num % ROWS_PER_PAGE;
//   uint32_t bytes_offset = row_offset * ROW_SIZE;
//   return page + bytes_offset;
// }
