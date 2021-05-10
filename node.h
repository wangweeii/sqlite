//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdint>
#include "save.h"

#ifndef SQLITE_NODE_H
#define SQLITE_NODE_H

struct Row;

struct Cell
{
        uint32_t key;
        Row      row;
};

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
        uint32_t         cell_nums;
};

NodeType get_node_type(void *node);

void set_node_type(void *node, NodeType type);

bool is_node_root(void *node);

void set_node_root(void *node, bool is_root);

void *node_parent(void *node);

// 读取叶子结点中共有多少个Cell
uint32_t leaf_node_cell_nums(void *node);

// 修改叶子结点中的Cell数量
void leaf_node_cell_nums(void *node, uint32_t num);

// 获取叶子结点中某一个键值对的指针
Cell *leaf_node_cell(void *node, uint32_t cell_num);

void initialize_leaf_node(void *node);

#endif //SQLITE_NODE_H