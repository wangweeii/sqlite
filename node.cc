//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "cursor.h"
#include "node.h"
#include "save.h"

// 所有节点通用操作函数
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

uint32_t get_node_max_key(void *node)
{
        switch (get_node_type(node))
        {
                case NODE_INTERNAL:
                        return internal_node_cell(node, internal_node_key_nums(node) - 1)->key;
                case NODE_LEAF:
                        return leaf_node_cell(node, leaf_node_cell_nums(node) - 1)->key;
        }
}

// 打印所有节点
void print_tree(void *node)
{
        switch (get_node_type(node))
        {
                case NODE_LEAF:
                        print_leaf_node(node);
                        break;
                case NODE_INTERNAL:
                        for (uint32_t i = 0; i != internal_node_key_nums(node); ++i)
                        {
                                print_tree(internal_node_child(node, i));
                        }
                        print_tree(internal_node_right_child(node));
        }
}

// 内部节点操作函数
// 获取内部节点中的key数量
uint32_t internal_node_key_nums(void *node)
{
        return ((InternalNodeHeader *) node)->key_nums;
}

// 获取内部节点的最右孩子
void *internal_node_right_child(void *node)
{
        return ((InternalNodeHeader *) node)->right_child;
}

// 获取内部节点中的InternalCell指针
InternalCell *internal_node_cell(void *node, uint32_t cell_num)
{
        return ((InternalCell *) ((InternalNodeHeader *) node + 1)) + cell_num;
}

// 获取内部节点中的某个子节点指针
void *internal_node_child(void *node, uint32_t child_num)
{
        uint32_t key_nums = internal_node_key_nums(node);
        if (child_num > key_nums)
        {
                printf("Tried to access child_num %d > key_nums %d\n", child_num, key_nums);
                exit(EXIT_FAILURE);
        }
        else if (child_num == key_nums)
        {
                return internal_node_right_child(node);
        }
        else
        {
                return internal_node_cell(node, child_num)->child;
        }
}

// 初始化内部节点
void initialize_internal_node(void *node)
{
        set_node_type(node, NODE_INTERNAL);
        set_node_root(node, false);
        ((InternalNodeHeader *) node)->key_nums    = 0;
        ((InternalNodeHeader *) node)->right_child = nullptr;
}

// 叶节点操作函数
// 获取叶子节点中的Cell数量
uint32_t leaf_node_cell_nums(void *node)
{
        return ((LeafNodeHeader *) node)->cell_nums;
}

// 修改叶子节点中的Cell数量
void leaf_node_cell_nums(void *node, uint32_t num)
{
        ((LeafNodeHeader *) node)->cell_nums = num;
}

// 获取叶子节点中某一个键值对Cell的指针
Cell *leaf_node_cell(void *node, uint32_t cell_num)
{
        return ((Cell *) ((LeafNodeHeader *) node + 1)) + cell_num;
}

void initialize_leaf_node(void *node)
{
        set_node_type(node, NODE_LEAF);
        set_node_root(node, false);
        ((LeafNodeHeader *) node)->cell_nums = 0;
}

void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *row)
{
        void     *old_node    = get_page(cursor->table->pager, cursor->page_num);
        uint32_t new_page_num = get_unused_page_num(cursor->table->pager);
        void     *new_node    = get_page(cursor->table->pager, new_page_num);
        initialize_leaf_node(new_node);


}

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *row)
{
        void     *node     = get_page(cursor->table->pager, cursor->page_num);
        uint32_t cells_num = leaf_node_cell_nums(node);
        if (cells_num >= MAX_CELLS_PER_LEAF)
        {
                // printf("Need to implement splitting a leaf node.\n");
                // exit(EXIT_FAILURE);
                leaf_node_split_and_insert(cursor, key, row);
                return;
        }

        if (cursor->cell_num < cells_num)
        {
                // 将cell_num这个位置腾出来
                for (uint32_t i = cells_num; i != cursor->cell_num; --i)
                {
                        memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1), sizeof(Cell));
                }
        }

        // 将key和row写入上一步腾出来的地方
        auto *cell = leaf_node_cell(node, cursor->cell_num);
        cell->key = key;
        cell->row = *row;
        ((LeafNodeHeader *) node)->cell_nums += 1;
}

Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key)
{
        void     *node     = get_page(table->pager, page_num);
        uint32_t cells_num = leaf_node_cell_nums(node);

        auto *cursor = static_cast<Cursor *>(malloc(sizeof(Cursor)));
        cursor->table    = table;
        cursor->page_num = page_num;

        // 二叉查找
        uint32_t min_index          = 0;
        uint32_t one_past_max_index = cells_num;
        while (one_past_max_index != min_index)
        {
                uint32_t middle       = (min_index + one_past_max_index) / 2;
                uint32_t key_at_index = leaf_node_cell(node, middle)->key;

                if (key == key_at_index)
                {
                        cursor->cell_num = middle;
                        break;
                }
                if (key < key_at_index)
                {
                        one_past_max_index = middle;
                }
                else
                {
                        min_index = middle + 1;
                }
        }

        return cursor;
}

void print_leaf_node(void *node)
{
        printf("%d Cells in this leaf node.\n", leaf_node_cell_nums(node));
        for (uint32_t i = 0; i != leaf_node_cell_nums(node); ++i)
        {
                printf("  - %d: %d\n", i, leaf_node_cell(node, i)->key);
        }
}