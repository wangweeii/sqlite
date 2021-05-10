//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdlib>
#include "cursor.h"
#include "node.h"

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

Cursor *table_end(Table *table)
{
        auto *cursor = static_cast<Cursor *>(malloc(sizeof(Cursor)));
        cursor->table    = table;
        cursor->page_num = table->root_page_num;

        void     *root_node = get_page(table->pager, table->root_page_num);
        uint32_t cell_nums  = leaf_node_cell_nums(root_node);

        cursor->cell_num     = cell_nums;
        cursor->end_of_table = true;

        return cursor;
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
