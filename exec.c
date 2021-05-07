//
// Created by Wang Wei on 2021/4/25.
//

#include "exec.h"
#include "table.h"

ExecuteResult execute_insert(Statement *statement, Table *table)
{
        if (table->num_rows >= TABLE_MAX_ROWS)
        {
                return EXECUTE_TABLE_FULL;
        }

        Row *row_to_insert = &(statement->row_to_insert);
        serialize_row(row_to_insert, row_slot(table, table->num_rows));
        table->num_rows += 1;

        return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
        Row row;
        for (uint32_t i = 0; i < table->num_rows; ++i)
        {
                deserialize_row(row_slot(table, i), &row);
                print_row(&row);
        }
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
