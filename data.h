//
// Created by Wang Wei on 2021/5/10.
//

#ifndef SQLITE_DATA_H
#define SQLITE_DATA_H

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 218
#define PAGE_SIZE 4096
#define MAX_PAGES_PER_TABLE 100

struct Row
{
        uint32_t id; // 要插入的ID
        char     username[COLUMN_USERNAME_SIZE + 1]; // 要插入的列名
        char     email[COLUMN_EMAIL_SIZE + 1];
};

struct Cell
{
        uint32_t key;
        Row      row;
};

// 分页器
struct Pager
{
        int      file_descriptor;
        uint32_t file_length;
        uint32_t num_pages;
        void     *pages[MAX_PAGES_PER_TABLE];
};

struct Table
{
        Pager    *pager;
        uint32_t root_page_num; // 根节点所在的页号
};

#endif //SQLITE_DATA_H
