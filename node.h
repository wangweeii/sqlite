//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdint>
#include "base.h"
// #include "save.h"
#include "cursor.h"

#ifndef SQLITE_NODE_H
#define SQLITE_NODE_H

enum NodeType : uint8_t
{
        NODE_INTERNAL,
        NODE_LEAF
};

struct CommonNodeHeader
{
        NodeType node_type;
        bool     is_root;
        void     *parent;
};

// 非叶子节点头
struct InternalNodeHeader
{
        CommonNodeHeader common_header;
        uint32_t         num_keys;
        void             *right_child;
};

// 叶子节点头
struct LeafNodeHeader
{
        CommonNodeHeader common_header;
        uint32_t         cell_nums;
};

constexpr uint32_t MAX_CELLS_PER_LEAF = (PAGE_SIZE - sizeof(LeafNodeHeader)) / sizeof(Cell);

NodeType get_node_type(void *node);

void set_node_type(void *node, NodeType type);

bool is_node_root(void *node);

void set_node_root(void *node, bool is_root);

void *node_parent(void *node);

// 获取叶子节点中的Cell数量
uint32_t leaf_node_cell_nums(void *node);

// 修改叶子节点中的Cell数量
void leaf_node_cell_nums(void *node, uint32_t num);

// 获取叶子节点中某一个键值对的指针
Cell *leaf_node_cell(void *node, uint32_t cell_num);

void initialize_leaf_node(void *node);

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *row);

void print_leaf_node(void *node);

#endif //SQLITE_NODE_H