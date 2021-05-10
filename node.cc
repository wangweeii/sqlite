//
// Created by Wang Wei on 2021/5/9.
//

#include "node.h"

NodeType get_node_type(void *node)
{
        return ((CommonNodeHeader *) node)->node_type;
}

void set_node_type(void *node, NodeType type)
{
        ((CommonNodeHeader *) node)->node_type = type;
}

bool is_node_root(void *node)
{
        return ((CommonNodeHeader *) node)->is_root;
}

void set_node_root(void *node, bool is_root)
{
        ((CommonNodeHeader *) node)->is_root = true;
}

void *node_parent(void *node)
{
        return ((CommonNodeHeader *) node)->parent;
}

// 读取叶子结点中共有多少个Cell
uint32_t leaf_node_cell_nums(void *node)
{
        return ((LeafNodeHeader *) node)->cell_nums;
}

// 修改叶子结点中的Cell数量
void leaf_node_cell_nums(void *node, uint32_t num)
{
        ((LeafNodeHeader *) node)->cell_nums = num;
}

// 获取叶子结点中某一个键值对的指针
Cell *leaf_node_cell(void *node, uint32_t cell_num)
{
        return ((Cell *) ((LeafNodeHeader *) node + 1)) + cell_num;
}

void initialize_leaf_node(void *node)
{
        set_node_type(node, NODE_LEAF);
        set_node_root(node, true);
        ((LeafNodeHeader *) node)->cell_nums = 0;
}
