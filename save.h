//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___TABLE_H
#define SQLITE___TABLE_H

#include <cstdint>
#include "base.h"
// #include "node.h"

void insert_row(Row *destination, Row *source);

void print_row(Row *row);

Pager *pager_open(const char *filename);

// 读取特定的某一页，如果该页不存在，就分配一页
void *get_page(Pager *pager, uint32_t page_num);

uint32_t get_unused_page_num(Pager*pager);

// 写入pager的第page_num页
void pager_flush(Pager *pager, uint32_t page_num);

Table *db_open(const char *filename);

void db_close(Table *table);

#endif //SQLITE___TABLE_H