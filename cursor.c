#include "cursor.h"
#include "btree.h"
#include "serializers.h"
#include <string.h>

Cursor *table_start(Table *table) {
  Cursor *cursor = table_find(table, 0);
  void *node = get_page(table->pager, cursor->page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);
  cursor->end_of_table = num_cells == 0;
  return cursor;
}

Cursor *table_find(Table *table, uint32_t key) {
  uint32_t root_page_num = table->root_page_num;
  void *root_node = get_page(table->pager, root_page_num);
  if (get_node_type(root_node) == NODE_LEAF) {
    return leaf_node_find(table, root_page_num, key);
  } else {
    return internal_node_find(table, root_page_num, key);
  }
}

uint32_t internal_node_find_child(void *node, uint32_t key) {
  uint32_t num_keys = *internal_node_num_keys(node);

  uint32_t min_index = 0;
  uint32_t max_index = num_keys;

  while (min_index != max_index) {
    uint32_t mid = (min_index + max_index) / 2;
    uint32_t key_at_mid = *internal_node_key(node, mid);
    if (key_at_mid >= key) {
      max_index = mid;
    } else {
      min_index = mid + 1;
    }
  }
  return min_index;
}

Cursor *internal_node_find(Table *table, uint32_t page_num, uint32_t key) {
  void *node = get_page(table->pager, page_num);
  uint32_t child_index = internal_node_find_child(node, key);
  uint32_t child_num = *internal_node_child(node, child_index);
  void *child = get_page(table->pager, child_num);

  switch (get_node_type(child)) {
  case NODE_LEAF:
    return leaf_node_find(table, child_num, key);
  case NODE_INTERNAL:
    return internal_node_find(table, child_num, key);
  }
}

void update_internal_node_key(void *node, uint32_t old_key, uint32_t new_key) {
  uint32_t old_child_index = internal_node_find_child(node, old_key);
  *internal_node_key(node, old_child_index) = new_key;
}

Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key) {
  void *node = get_page(table->pager, page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);

  Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->page_num = page_num;

  uint32_t min_index = 0;
  uint32_t one_past_max_index = num_cells;

  while (min_index != one_past_max_index) {
    uint32_t mid = (min_index + one_past_max_index) / 2;
    uint32_t key_at_index = *leaf_node_key(node, mid);
    if (key == key_at_index) {
      cursor->cell_num = mid;
      return cursor;
    } else if (key < key_at_index) {
      one_past_max_index = mid;
    } else {
      min_index = mid + 1;
    }
  }

  cursor->cell_num = min_index;
  return cursor;
}

void internal_node_insert(Table *table, uint32_t parent_page_num,
                          uint32_t child_page_num) {
  void *parent = get_page(table->pager, parent_page_num);
  void *child = get_page(table->pager, child_page_num);
  uint32_t child_max_key = get_node_max(child);
  uint32_t index = internal_node_find_child(parent, child_max_key);

  uint32_t original_num_keys = *internal_node_num_keys(parent);
  *internal_node_num_keys(parent) = original_num_keys + 1;
  if (original_num_keys >= INTERNAL_NODE_MAX_KEYS) {
    printf("Need to implement splitting internal node\n");
    exit(EXIT_FAILURE);
  }

  uint32_t right_child_page_num = *internal_node_right_child(parent);
  void *right_child = get_page(table->pager, right_child_page_num);

  if (child_max_key > get_node_max(right_child)) {
    *internal_node_child(parent, original_num_keys) = right_child_page_num;
    *internal_node_key(parent, original_num_keys) = get_node_max(right_child);
    *internal_node_right_child(parent) = child_page_num;
  } else {
    for (uint32_t i = original_num_keys; i > index; i--) {
      void *destination = internal_node_cell(parent, i);
      void *source = internal_node_cell(parent, i - 1);
      memcpy(destination, source, INTERNAL_NODE_CELL_SIZE);
    }
  }
}

void leaf_node_split_insert(Cursor *cursor, uint32_t key, Row *row) {
  /*
   * Create a new node and move half cells over
   * Insert the row in one of the two nodes
   * Update parent of create a new parent
   */

  void *old_node = get_page(cursor->table->pager, cursor->page_num);
  uint32_t old_max = get_node_max(old_node);
  uint32_t new_page_num = get_unused_page_num(cursor->table->pager);
  void *new_node = get_page(cursor->table->pager, new_page_num);
  init_leaf_node(new_node);
  *node_parent(new_node) = *node_parent(old_node);

  /* Update next leaf nodes */
  *leaf_node_next_leaf(new_node) = *leaf_node_next_leaf(old_node);
  *leaf_node_next_leaf(old_node) = new_page_num;

  /*
   * The code below will insure that the keys are properly divided between the
   * two new nodes
   */
  for (int32_t i = LEAF_NODE_MAX_CELLS; i >= 0; i--) {
    void *destination_node;
    if (i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
      destination_node = new_node;
    } else {
      destination_node = old_node;
    }
    /*

     *You will notice that we also insure that the keys remain
     * sorted while inserting
     */
    uint32_t index_within_node = i % LEAF_NODE_LEFT_SPLIT_COUNT;
    void *destination = leaf_node_cell(destination_node, index_within_node);

    if (i == cursor->cell_num) {
      serialize(leaf_node_value(destination_node, index_within_node), row);
      *leaf_node_key(destination_node, index_within_node) = key;
    } else if (i > cursor->cell_num) {
      memcpy(destination, leaf_node_cell(old_node, i - 1), LEAF_NODE_CELL_SIZE);
    } else {
      memcpy(destination, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
    }
  }
  /* Update cell count on both leafs */
  *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT_COUNT;
  *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

  if (is_root_node(old_node)) {
    return create_new_root(cursor->table, new_page_num);
  } else {
    uint32_t parent_page_num = *node_parent(old_node);
    uint32_t new_max = get_node_max(old_node);
    void *parent = get_page(cursor->table->pager, parent_page_num);

    update_internal_node_key(parent, old_max, new_max);
    internal_node_insert(cursor->table, parent_page_num, new_page_num);
    return;
  }
}

void cursor_advance(Cursor *cursor) {
  uint32_t page_num = cursor->page_num;
  void *node = get_page(cursor->table->pager, page_num);
  cursor->cell_num += 1;
  if (cursor->cell_num >= *leaf_node_num_cells(node)) {
    uint32_t next_page_num = *leaf_node_next_leaf(node);
    if (next_page_num == 0) {
      /* This means we have reached the last lead node */
      cursor->end_of_table = true;
    } else {
      cursor->page_num = next_page_num;
      cursor->cell_num = 0;
    }
  }
}

void *cursor_value(Cursor *cursor) {
  uint32_t page_num = cursor->page_num;
  void *page = get_page(cursor->table->pager, page_num);
  return leaf_node_value(page, cursor->cell_num);
}
