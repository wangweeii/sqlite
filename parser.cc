//
// Created by Wang Wei on 2021/5/7.
//

#include "parser.h"

// 将输入的字符串解析为SQL语句
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
        // 先判断是不是insert语句
        if (strncmp(input_buffer->buffer, "insert", 6) == 0)
        {
                statement->type = STATEMENT_INSERT;
                int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
                                           statement->row_to_insert.username, statement->row_to_insert.email);
                if (args_assigned < 3)
                {
                        return PREPARE_SYNTAX_ERROR;
                }
                return PREPARE_SUCCESS;
        }
        // 再判断是不是select语句
        if (strncmp(input_buffer->buffer, "select", 6) == 0)
        {
                statement->type = STATEMENT_SELECT;
                return PREPARE_SUCCESS;
        }
        // 返回未知语句错误
        return PREPARE_UNRECOGNIZED_STATEMENT;
}