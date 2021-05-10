//
// Created by Wang Wei on 2021/5/7.
//

#include <iostream>
#include <cstring>
#include <sys/file.h>
#include <unistd.h>
#include "save.h"

void insert_row(Row *destination, Row *source)
{
        destination->id = source->id;
        strncpy(destination->username, source->username, sizeof(source->username));
        strncpy(destination->email, source->email, sizeof(source->email));
}

void print_row(Row *row)
{
        printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

// 打开文件，并创建一个分页器与之关联起来
Pager *pager_open(const char *filename)
{
        int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
        if (fd == -1)
        {
                printf("Unable to open file\n");
                exit(EXIT_FAILURE);
        }

        // 使用SEEK_END表示将打开点移到文件最后加0的地方
        off_t file_length = lseek(fd, 0, SEEK_END);

        auto *pager = static_cast<Pager *>(malloc(sizeof(Pager)));
        pager->file_descriptor = fd;
        pager->file_length     = file_length;
        pager->num_pages       = file_length / PAGE_SIZE;

        if (file_length % PAGE_SIZE != 0)
        {
                printf("DataBase file is not a whole number of pages. Corrupt file.\n");
                exit(EXIT_FAILURE);
        }

        for (uint32_t i = 0; i != MAX_PAGES_PER_TABLE; ++i)
        {
                pager->pages[i] = nullptr;
        }

        return pager;
}

// 读取特定的某一页
void *get_page(Pager *pager, uint32_t page_num)
{
        if (page_num > MAX_PAGES_PER_TABLE)
        {
                printf("Tried to fetch page number out of bounds. %d > %d\n", page_num, MAX_PAGES_PER_TABLE);
                exit(EXIT_FAILURE);
        }

        if (pager->pages[page_num] == nullptr)
        {
                void     *page     = malloc(sizeof(Pager));
                uint32_t num_pages = pager->file_length / sizeof(Pager);

                // 如果文件长度对页大小取余有剩的，说明后面还能再放一页
                if (pager->file_length % sizeof(Pager))
                {
                        num_pages += 1;
                }

                // 要取的页没有超过总页数，将文件中对应的内容读到这一页中
                if (page_num <= num_pages)
                {
                        lseek(pager->file_descriptor, page_num * sizeof(Pager), SEEK_SET);
                        ssize_t bytes_read = read(pager->file_descriptor, page, sizeof(Pager));
                        if (bytes_read == -1)
                        {
                                printf("Error reading file: %d\n", errno);
                                exit(EXIT_FAILURE);
                        }
                }

                pager->pages[page_num] = page;
                if (page_num >= pager->num_pages)
                {
                        pager->num_pages = page_num + 1;
                }
        }

        return pager->pages[page_num];
}

// 写入pager的第page_num页
void pager_flush(Pager *pager, uint32_t page_num)
{
        if (pager->pages[page_num] == nullptr)
        {
                printf("Tried to flush null page.\n");
                exit(EXIT_FAILURE);
        }

        ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], PAGE_SIZE);
        if (bytes_written == -1)
        {
                printf("Error writing: %d\n", errno);
                exit(EXIT_FAILURE);
        }
}

// 打开数据库文件
Table *db_open(const char *filename)
{
        Pager *pager = pager_open(filename);
        auto  *table = static_cast<Table *>(malloc(sizeof(Table)));
        table->pager = pager;
        table->root_page_num = 0;

        if (pager->num_pages == 0)
        {
                void *root_node = get_page(pager, 0);
                initialize_leaf_node(root_node);
        }

        return table;
}

void db_close(Table *table)
{
        Pager         *pager = table->pager;
        for (uint32_t i      = 0; i != pager->num_pages; ++i)
        {
                // 只写回已从文件中读取出来的页，没读出来的指针为空
                if (pager->pages[i] == nullptr)
                {
                        continue;
                }
                pager_flush(pager, i);
        }

        int result = close(pager->file_descriptor);
        if (result == -1)
        {
                printf("Error closing db file.\n");
                exit(EXIT_FAILURE);
        }
        // 此处统一释放所有页占用的内存
        for (uint32_t i = 0; i != MAX_PAGES_PER_TABLE; ++i)
        {
                if (pager->pages[i])
                {
                        free(pager->pages[i]);
                        pager->pages[i] = nullptr;
                }
        }
        free(pager);
        free(table);
}
