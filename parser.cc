//
// Created by Wang Wei on 2021/5/7.
//

#include <string>
#include "parser.h"

// 准备insert语句
PrepareResult prepare_insert(InputBuffer *input_buffer, Statement *statement)
{
        statement->type = STATEMENT_INSERT;

        std::string keyword = strtok(input_buffer->buffer, " ");
        std::string id_string = strtok(nullptr, " ");
        std::string username = strtok(nullptr, " ");
        std::string email = strtok(nullptr, " ");

        if (id_string.empty() || username.empty() || email.empty())
        {
                return PREPARE_SYNTAX_ERROR;
        }

        int32_t id = atoi(id_string.c_str());
        if (id < 0)
        {
                return PREPARE_NEGATIVE_ID;
        }

        if (username.length() > COLUMN_USERNAME_SIZE || email.length() > COLUMN_EMAIL_SIZE)
        {
                return PREPARE_STRING_TOO_LONG;
        }

        statement->row_to_insert.id = id;
        strcpy(statement->row_to_insert.username, username.c_str());
        strcpy(statement->row_to_insert.email, email.c_str());

        return PREPARE_SUCCESS;
}

// 将输入的字符串解析为SQL语句
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
        // 先判断是不是insert语句
        if (strncmp(input_buffer->buffer, "insert", 6) == 0)
        {
                return prepare_insert(input_buffer, statement);
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