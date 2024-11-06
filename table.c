#include "table.h"
#include "pager.h"
#include <string.h>

Table *open_db() {
  Table *table = (Table *)malloc(sizeof(Table));
  Pager *pager = open_pager();
  table->root_page_num = 0;
  table->pager = pager;

  // If the db file is empty we initialize a root node
  if (pager->num_pages == 0) {
    void *root_node = get_page(pager, 0);
    init_leaf_node(root_node);
  }

  return table;
}

void close_db(Table *table) {
  Pager *pager = table->pager;

  for (int page_num = 0; page_num < pager->num_pages; page_num++) {
    void *page = pager->pages[page_num];
    if (page == NULL) {
      continue;
    }
    flush_page(pager, page_num);
    free(page);
    pager->pages[page_num] = NULL;
  }

  close(pager->file_descriptor);
  free(pager);
  free(table);
}

void create_new_root(Table *table, uint32_t right_child_page_num) {
  /* Handle splitting the root
   * Old root needs to be copied to the new page (left one)
   */

  void *root = get_page(table->pager, table->root_page_num);
  void *right_child = get_page(table->pager, right_child_page_num);
  uint32_t left_child_num = get_unused_page_num(table->pager);
  void *left_child = get_page(table->pager, left_child_num);
  memcpy(left_child, root, PAGE_SIZE);
  set_root_node(left_child, false);
}
