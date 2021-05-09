//
// Created by Wang Wei on 2021/5/7.
//

#include <cstdlib>
#include "exec.h"
#include "save.h"
#include "cursor.h"

ExecuteResult execute_insert(Statement *statement, Table *table)
{
        if (table->num_rows >= MAX_ROWS_PER_TABLE)
        {
                return EXECUTE_TABLE_FULL;
        }

        // 写入一行
        Cursor *cursor = table_end(table);
        insert_row(cursor_value(cursor), &(statement->row_to_insert));
        table->num_rows += 1;

        free(cursor);

        return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
        Cursor *cursor = table_start(table);
        while (!(cursor->end_of_table))
        {
                print_row(cursor_value(cursor));
                cursor_advance(cursor);
        }

        free(cursor);

        return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table *table)
{
        switch (statement->type)
        {
                case STATEMENT_INSERT:
                        // printf("This is where we would do an insert.\n");
                        // break;
                        return execute_insert(statement, table);
                case STATEMENT_SELECT:
                        // printf("This is where we would do a select.\n");
                        // break;
                        return execute_select(statement, table);
        }
}