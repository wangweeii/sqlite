//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdint>
#include "save.h"

#ifndef SQLITE_NODE_H
#define SQLITE_NODE_H

typedef enum
{
        NODE_INTERNAL,
        NODE_LEAF
} NodeType;

// Common Node Header Layout
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint64_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_SIZE + IS_ROOT_OFFSET;
const uint8_t COMMON_MODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + IS_ROOT_OFFSET;

// Leaf Node Header Layout
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_MODE_HEADER_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_MODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

// Leaf Node body Layout
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_VALUE_SIZE = sizeof(Row);

#endif //SQLITE_NODE_H
