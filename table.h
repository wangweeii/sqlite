//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___TABLE_H
#define SQLITE___TABLE_H

#include <cstdint>
#include "row.h"

#define MAX_PAGES_PER_TABLE 100
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * MAX_PAGES_PER_TABLE;

struct Table
{
        uint32_t num_rows;
        Row *pages[MAX_PAGES_PER_TABLE];
};

Row *row_slot(Table *table, uint32_t row_num);

Table *new_table();

void free_table(Table *table);

#endif //SQLITE___TABLE_H
