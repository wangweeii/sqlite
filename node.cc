//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "cursor.h"
#include "node.h"
#include "save.h"

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

// 获取叶子节点中某一个键值对的指针
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