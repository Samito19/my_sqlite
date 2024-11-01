#ifndef BTREE_H
#define BTREE_H

#include "serializers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

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
 * Leaf Node Header Layout
 */

const static uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const static uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const static uint32_t LEAF_NODE_HEADER_SIZE =
    COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

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

uint32_t *leaf_node_num_cells(void *);
void *leaf_node_cell(void *, uint32_t);
uint32_t *leaf_node_key(void *, uint32_t);
void *leaf_node_value(void *, uint32_t);
void init_leaf_node(void *);
void print_leaf_node(void *);

#endif // !BTREE_H