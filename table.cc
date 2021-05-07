//
// Created by Wang Wei on 2021/5/6.
//

#include <stdlib.h>
#include "table.h"

// 查找当前行在表中的位置
void *row_slot(Table *table, uint32_t row_num)
{
        uint32_t page_num = row_num / ROWS_PER_PAGE;
        void *page = table->pages[page_num];
        // 当该页为空时就创建一个新的页
        if (page == NULL)
        {
                table->pages[page_num] = malloc(PAGE_SIZE);
                page = table->pages[page_num];
        }
        uint32_t row_offset = row_num % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;
        return page + byte_offset;
}

Table *new_table()
{
        Table *table = malloc(sizeof(Table));
        table->num_rows = 0;
        for (uint32_t i = 0; i < TABLE_MAX_ROWS; ++i)
        {
                table->pages[i] = NULL;
        }
        return table;
}

void free_table(Table *table)
{
        for (uint32_t i = 0; table->pages[i]; ++i)
        {
                free(table->pages[i]);
        }
        free(table);
}