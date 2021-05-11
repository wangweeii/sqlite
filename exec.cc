//
// Created by Wang Wei on 2021/5/7.
//

#include <cstdlib>
#include "exec.h"
#include "node.h"
#include "save.h"
#include "cursor.h"

ExecuteResult execute_insert(Statement *statement, Table *table)
{
        void *node = get_page(table->pager, table->root_page_num);
        if (leaf_node_cell_nums(node) >= MAX_CELLS_PER_LEAF)
        {
                return EXECUTE_TABLE_FULL;
        }

        // 写入一行
        Cursor *cursor = table_end(table);
        // leaf_node_insert(cursor, statement->row_to_insert.id, &(statement->row_to_insert));
        leaf_node_insert(table_end(table), statement->row_to_insert.id, &(statement->row_to_insert));

        free(cursor);

        return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
        Cursor *cursor = table_start(table);
        while (!(cursor->end_of_table))
        {
                print_row((Row *) cursor_value(cursor));
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