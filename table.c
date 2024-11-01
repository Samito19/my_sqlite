#include "table.h"

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
