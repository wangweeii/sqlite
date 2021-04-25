//
// Created by wangwei on 04/25.
//

#ifndef SQLITE_PARSER_H
#define SQLITE_PARSER_H

#include <stdint.h>
#include "input.h"

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

// 语句类型：insert, select
typedef enum
{
        STATEMENT_INSERT,
        STATEMENT_SELECT
} StatementType;

typedef struct
{
        uint32_t id; // 要插入的ID
        char username[COLUMN_USERNAME_SIZE]; // 要插入的列名
        char email[COLUMN_EMAIL_SIZE];
} Row;

// SQL语句
typedef struct
{
        StatementType type;
        Row row_to_insert;
} Statement;

// 解析SQL语句的返回值
typedef enum
{
        PREPARE_SUCCESS,
        PREPARE_SYNTAX_ERROR,
        PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);

#endif //SQLITE_PARSER_H