//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___EXEC_H
#define SQLITE___EXEC_H

#include "parser.h"
#include "save.h"

typedef enum
{
        EXECUTE_SUCCESS,
        EXECUTE_DUPLICATE_KEY,
        EXECUTE_TABLE_FULL
} ExecuteResult;

ExecuteResult execute_insert(Statement *statement, Table *table);

ExecuteResult execute_select(Statement *statement, Table *table);

ExecuteResult execute_statement(Statement *statement, Table *table);

#endif //SQLITE___EXEC_H
