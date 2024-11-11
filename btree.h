#ifndef BTREE_H
#define BTREE_H

#include "constants.h"
#include "pager.h"
#include "serializers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

uint32_t *leaf_node_num_cells(void *);
void *leaf_node_cell(void *, uint32_t);
uint32_t *leaf_node_key(void *, uint32_t);
void *leaf_node_value(void *, uint32_t);
void init_leaf_node(void *);
void print_tree(Pager *, uint32_t, uint32_t);
NodeType get_node_type(void *);
bool is_root_node(void *);
void set_root_node(void *, bool);
void init_internal_node(void *);
uint32_t *internal_node_num_keys(void *);
uint32_t *internal_node_right_child(void *);
uint32_t *internal_node_cell(void *, uint32_t);
uint32_t *internal_node_child(void *, uint32_t);
uint32_t *internal_node_key(void *, uint32_t);
uint32_t get_node_max(void *);
uint32_t *leaf_node_next_leaf(void *);
uint32_t *node_parent(void *);

/*
 * Common Node Header Layout
 */

const static uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const static uint32_t NODE_TYPE_OFFSET = 0;
const static uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const static uint32_t IS_ROOT_OFFSET = NODE_TYPE_OFFSET + NODE_TYPE_SIZE;
const static uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const static uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const static uint8_t COMMON_NODE_HEADER_SIZE =
    NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

/*
 * Internal Node Header Layout
 */

const static uint32_t INTERNAL_NODE_NUM_KEYS_SIZE = sizeof(uint32_t);
const static uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
const static uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(uint32_t);
const static uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET =
    INTERNAL_NODE_NUM_KEYS_OFFSET + INTERNAL_NODE_NUM_KEYS_SIZE;
const static uint32_t INTERNAL_NODE_HEADER_SIZE =
    COMMON_NODE_HEADER_SIZE + INTERNAL_NODE_NUM_KEYS_SIZE +
    INTERNAL_NODE_RIGHT_CHILD_SIZE;

/*
 * Internal Node Body Layout
 */

const static uint32_t INTERNAL_NODE_KEY_SIZE = sizeof(uint32_t);
const static uint32_t INTERNAL_NODE_CHILD_SIZE = sizeof(uint32_t);
const static uint32_t INTERNAL_NODE_CELL_SIZE =
    INTERNAL_NODE_KEY_SIZE + INTERNAL_NODE_CHILD_SIZE;
// const static uint32_t INTERNAL_NODE_MAX_KEYS =
//     (PAGE_SIZE - INTERNAL_NODE_HEADER_SIZE) / INTERNAL_NODE_CELL_SIZE;
const static uint32_t INTERNAL_NODE_MAX_KEYS = 3;

/*
 * Leaf Node Header Layout
 */

const static uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const static uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const static uint32_t LEAF_NODE_NEXT_LEAF_SIZE = sizeof(uint32_t);
const static uint32_t LEAF_NODE_NEXT_LEAF_OFFSET =
    LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const static uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE +
                                              LEAF_NODE_NUM_CELLS_SIZE +
                                              LEAF_NODE_NEXT_LEAF_SIZE;

/*
 * Leaf Node Body Layout
 */

const static uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const static uint32_t LEAF_NODE_KEY_OFFSET = LEAF_NODE_KEY_SIZE;
const static uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const static uint32_t LEAF_NODE_VALUE_OFFSET =
    LEAF_NODE_KEY_OFFSET + LEAF_NODE_VALUE_SIZE;
const static uint32_t LEAF_NODE_CELL_SIZE =
    LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const static uint32_t LEAF_NODE_SPACE_FOR_CELLS =
    PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const static uint32_t LEAF_NODE_MAX_CELLS =
    LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;
const static uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT =
    (LEAF_NODE_MAX_CELLS + 1) / 2;
const static uint32_t LEAF_NODE_LEFT_SPLIT_COUNT =
    (LEAF_NODE_MAX_CELLS + 1) - LEAF_NODE_RIGHT_SPLIT_COUNT;

#endif // !BTREE_H
