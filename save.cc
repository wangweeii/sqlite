//
// Created by Wang Wei on 2021/5/7.
//

#include <iostream>
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

// 打开文件，并创建一个内存页与之关联起来
Pager *pager_open(const char *filename)
{
        int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
        if (fd == -1)
        {
                printf("Unable to open file\n");
                exit(EXIT_FAILURE);
        }

        off_t file_length = lseek(fd, 0, SEEK_END);

        auto *pager = static_cast<Pager *>(malloc(sizeof(Pager)));
        pager->file_descriptor = fd;
        pager->file_length = file_length;

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
                void *page = malloc(sizeof(Pager));
                uint32_t num_pages = pager->file_length / sizeof(Pager);

                // 如果文件长度对页大小取余有剩的，说明后面还能再放一页
                if (pager->file_length % sizeof(Pager))
                {
                        num_pages += 1;
                }

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
        }

        return pager->pages[page_num];
}

// 写入pager的第page_num页
void pager_flush(Pager *pager, uint32_t page_num, uint32_t size)
{
        if (pager->pages[page_num] == nullptr)
        {
                printf("Tried to flush null page.\n");
                exit(EXIT_FAILURE);
        }

        // 定位到要写入的页在文件中的偏移量
        off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
        if (offset == -1)
        {
                printf("Error seeking: %d\n", errno);
                exit(EXIT_FAILURE);
        }

        ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], size);
        if (bytes_written == -1)
        {
                printf("Error writing: %d\n", errno);
                exit(EXIT_FAILURE);
        }
}

// 打开数据库文件
Table *db_open(const char *filename)
{
        auto *table = static_cast<Table *>(malloc(sizeof(Table)));

        Pager *pager = pager_open(filename);
        uint32_t num_rows = pager->file_length / sizeof(Row);

        table->pager = pager;
        table->num_rows = num_rows;

        return table;
}

void db_close(Table *table)
{
        Pager *pager = table->pager;
        // 这个变量名表示写满的页
        uint32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;
        for (uint32_t i = 0; i != num_full_pages; ++i)
        {
                // 只写回已从文件中读取出来的页，没读出来的指针为空
                if (pager->pages[i] == nullptr)
                {
                        continue;
                }
                pager_flush(pager, i, PAGE_SIZE);
        }

        // 还要把最后不足一页的数据写入到文件中
        uint32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
        if (num_additional_rows > 0)
        {
                uint32_t page_num = num_full_pages;
                pager_flush(pager, page_num, num_additional_rows * sizeof(Row));
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
