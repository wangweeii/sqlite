//
// Created by Wang Wei on 2021/5/9.
//

#ifndef SQLITE___CURSOR_H
#define SQLITE___CURSOR_H

#include "base.h"

struct Cursor
{
        Table    *table;
        uint32_t page_num;
        uint32_t cell_num;
        bool     end_of_table;
};

// 表的起始游标
Cursor *table_start(Table *table);

// 从表中找到key所在的游标
Cursor *table_find(Table *table, uint32_t key);

void *cursor_value(Cursor *cursor);

void cursor_advance(Cursor *cursor);

#endif //SQLITE___CURSOR_H
