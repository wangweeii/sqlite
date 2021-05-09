//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___TABLE_H
#define SQLITE___TABLE_H

#include <cstdint>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 218
#define PAGE_SIZE 4096
#define MAX_PAGES_PER_TABLE 100

struct Row
{
        uint32_t id; // 要插入的ID
        char username[COLUMN_USERNAME_SIZE + 1]; // 要插入的列名
        char email[COLUMN_EMAIL_SIZE + 1];
};

struct Pager
{
        int file_descriptor;
        uint32_t file_length;
        // void *pages[MAX_PAGES_PER_TABLE];
        void *pages[MAX_PAGES_PER_TABLE];
};

const uint32_t ROWS_PER_PAGE = PAGE_SIZE / sizeof(Row);
const uint32_t MAX_ROWS_PER_TABLE = ROWS_PER_PAGE * MAX_PAGES_PER_TABLE;

struct Table
{
        uint32_t num_rows;
        Pager *pager;
};

void insert_row(Row *destination, Row *source);

void print_row(Row *row);

Pager *pager_open(const char *filename);

void *get_page(Pager *pager, uint32_t page_num);

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size);

Table *db_open(const char *filename);

void db_close(Table *table);

#endif //SQLITE___TABLE_H