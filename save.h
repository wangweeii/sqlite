//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___TABLE_H
#define SQLITE___TABLE_H

#include <cstdint>
#include "data.h"
// #include "node.h"

void insert_row(Row *destination, Row *source);

void print_row(Row *row);

Pager *pager_open(const char *filename);

void *get_page(Pager *pager, uint32_t page_num);

// 写入pager的第page_num页
void pager_flush(Pager *pager, uint32_t page_num);

Table *db_open(const char *filename);

void db_close(Table *table);

#endif //SQLITE___TABLE_H