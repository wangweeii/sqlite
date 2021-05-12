//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdlib>
#include <cstdio>
#include "cursor.h"
#include "node.h"
#include "save.h"

// 表的起始游标
Cursor *table_start(Table *table)
{
        auto *cursor = static_cast<Cursor *>(malloc(sizeof(Cursor)));
        cursor->table    = table;
        cursor->page_num = table->root_page_num;
        cursor->cell_num = 0;

        void     *root_node = get_page(table->pager, table->root_page_num);
        uint32_t cell_nums  = leaf_node_cell_nums(root_node);
        cursor->end_of_table = (cell_nums == 0);

        return cursor;
}

// 从表中找到key所在的游标
Cursor *table_find(Table *table, uint32_t key)
{
        void *root_node = get_page(table->pager, table->root_page_num);

        if (get_node_type(root_node) == NODE_LEAF)
        {
                return leaf_node_find(table, table->root_page_num, key);
        }
        else
        {
                printf("Need to implement searching an internal node\n");
                exit(EXIT_FAILURE);
        }
}

void *cursor_value(Cursor *cursor)
{
        // 先读取游标中的这一页
        void *page = get_page(cursor->table->pager, cursor->page_num);

        return &(leaf_node_cell(page, cursor->cell_num)->row);
}

void cursor_advance(Cursor *cursor)
{
        void *page = get_page(cursor->table->pager, cursor->page_num);

        cursor->cell_num += 1;
        if (cursor->cell_num >= leaf_node_cell_nums(page))
        {
                cursor->end_of_table = true;
        }
}
