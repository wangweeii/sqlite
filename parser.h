//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___PARSER_H
#define SQLITE___PARSER_H

#include "row.h"
#include "input.h"

// 语句类型：insert, select
typedef enum
{
        STATEMENT_INSERT,
        STATEMENT_SELECT
} StatementType;

// SQL语句
struct Statement
{
        StatementType type;
        Row row_to_insert;
};

// 解析SQL语句的返回值
typedef enum
{
        PREPARE_SUCCESS,
        PREPARE_SYNTAX_ERROR,
        PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);

#endif //SQLITE___PARSER_H
