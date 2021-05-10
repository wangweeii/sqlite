//
// Created by Wang Wei on 2021/5/9.
//

#ifndef SQLITE___CURSOR_H
#define SQLITE___CURSOR_H

#include "save.h"

struct Cursor
{
        Table *table;
        uint32_t page_num;
        uint32_t cell_num;
        bool end_of_table;
};

Cursor *table_start(Table *table);

Cursor *table_end(Table *table);

Row *cursor_value(Cursor *cursor);

void cursor_advance(Cursor *cursor);

#endif //SQLITE___CURSOR_H
