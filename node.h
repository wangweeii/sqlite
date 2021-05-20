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

// 内部节点头
struct InternalNodeHeader
{
        CommonNodeHeader common_header;
        uint32_t         key_nums;
        uint32_t         right_child;
};

// 叶子节点头
struct LeafNodeHeader
{
        CommonNodeHeader common_header;
        uint32_t         cell_nums;
};

constexpr uint32_t MAX_CELLS_PER_LEAF          = (PAGE_SIZE - sizeof(LeafNodeHeader)) / sizeof(Cell);
// 分割叶节点时右侧分多少Cell
constexpr uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT = (MAX_CELLS_PER_LEAF + 1) / 2;
constexpr uint32_t LEAF_NODE_LEFT_SPLIT_COUNT  = MAX_CELLS_PER_LEAF + 1 - LEAF_NODE_RIGHT_SPLIT_COUNT;

NodeType get_node_type(void *node);

// 通用节点操作函数
void set_node_type(void *node, NodeType type);

bool is_node_root(void *node);

void set_node_root(void *node, bool is_root);

void *node_parent(void *node);

uint32_t get_node_max_key(void *node);

// 打印所有节点
void print_tree(void *node);


// 内部节点操作函数
// 获取内部节点中的key数量
uint32_t internal_node_key_nums(void *node);

// 设置内部节点的key数量
void internal_node_key_nums(void *node, uint32_t key_nums);

// 获取内部节点的最右孩子
void *internal_node_right_child(void *node);

// 获取内部节点中的InternalCell指针
InternalCell *internal_node_cell(void *node, uint32_t cell_num);

// 获取内部节点中的某个子节点指针
void *internal_node_child(void *node, uint32_t child_num);

// 初始化内部节点
void initialize_internal_node(void *node);


// 叶节点操作函数
// 获取叶子节点中的Cell数量
uint32_t leaf_node_cell_nums(void *node);

// 修改叶子节点中的Cell数量
void leaf_node_cell_nums(void *node, uint32_t num);

// 获取叶子节点中某一个键值对的指针
Cell *leaf_node_cell(void *node, uint32_t cell_num);

void initialize_leaf_node(void *node);

void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *row);

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *row);

Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);

void print_leaf_node(void *node);

#endif //SQLITE_NODE_H