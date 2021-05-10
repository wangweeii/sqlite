//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdint>
#include "save.h"

#ifndef SQLITE_NODE_H
#define SQLITE_NODE_H

struct Row;

enum NodeType : uint8_t
{
        NODE_INTERNAL,
        NODE_LEAF
};

// Common Node Header Layout
const uint32_t NODE_TYPE_OFFSET        = 0;
const uint32_t NODE_TYPE_SIZE          = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET          = NODE_TYPE_SIZE;
const uint32_t IS_ROOT_SIZE            = sizeof(uint8_t);
const uint32_t PARENT_POINTER_OFFSET   = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint32_t PARENT_POINTER_SIZE     = sizeof(uint64_t);
const uint8_t  COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

// Leaf Node Header Layout
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_NUM_CELLS_SIZE   = sizeof(uint32_t);
const uint32_t LEAF_NODE_HEADER_SIZE      = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

// Leaf Node body Layout
const uint32_t LEAF_NODE_KEY_SIZE   = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_VALUE_SIZE      = sizeof(Row);

struct CommonNodeHeader
{
        NodeType node_type;
        bool     is_root;
        void     *parent;
};

// 非叶子结点头
struct InternalNodeHeader
{
        CommonNodeHeader common_header;
        uint32_t         num_keys;
        void             *right_child;
};

// 叶子结点头
struct LeafNodeHeader
{
        CommonNodeHeader common_header;
        uint32_t         cell_num;
};

NodeType get_node_type(void *node);

void set_node_type(void *node, NodeType type);

bool is_node_root(void *node);

void set_node_root(void *node, bool is_root);

void *node_parent(void *node);

void initialize_leaf_node(void *node);

#endif //SQLITE_NODE_H