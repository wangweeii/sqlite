//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___ROW_H
#define SQLITE___ROW_H

#include <cstdint>
#include <iostream>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 220

struct Row
{
        uint32_t id; // 要插入的ID
        char username[COLUMN_USERNAME_SIZE]; // 要插入的列名
        char email[COLUMN_EMAIL_SIZE];
};

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)
const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + USERNAME_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

static void print_row(Row *row)
{
        printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

void serialize_row(Row *source, Row *destination);

void deserialize_row(Row *source, Row *destination);

#endif //SQLITE___ROW_H
