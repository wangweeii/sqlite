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

void initialize_leaf_node(void *node)
{
        set_node_type(node, NODE_LEAF);
        set_node_root(node, true);
        ((LeafNodeHeader *) node)->cell_num = 0;
}