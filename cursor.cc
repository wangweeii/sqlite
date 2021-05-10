//
// Created by Wang Wei on 2021/5/9.
//

#include <cstdlib>
#include "cursor.h"

Cursor *table_start(Table *table)
{
        auto *cursor = static_cast<Cursor *>(malloc(sizeof(Cursor)));
        cursor->table = table;
        // cursor->row_num = 0;
        // cursor->end_of_table = (table->num_rows == 0);
        cursor->page_num = table->root_page_num;
        cursor->cell_num = 0;

        void *root_node = get_page(table->pager, table->root_page_num);

        return cursor;
}

Cursor *table_end(Table *table)
{
        auto *cursor = static_cast<Cursor *>(malloc(sizeof(Cursor)));
        cursor->table = table;
        cursor->row_num = table->num_rows;
        cursor->end_of_table = true;

        return cursor;
}

Row *cursor_value(Cursor *cursor)
{
        // 先计算出目标行所在页的起始地址
        Row *page = static_cast<Row *>(get_page(cursor->table->pager, cursor->row_num / ROWS_PER_PAGE));
        // 再计算出目标行是所在页的第几行
        uint32_t row_offset = cursor->row_num % ROWS_PER_PAGE;

        return page + row_offset;
}

void cursor_advance(Cursor *cursor)
{
        cursor->row_num += 1;
        cursor->end_of_table = (cursor->row_num >= cursor->table->num_rows);
}
