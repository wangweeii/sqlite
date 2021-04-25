//
// Created by Wang Wei on 2021/4/25.
//

#include "exec.h"

void execute_statement(Statement *statement)
{
        switch (statement->type)
        {
                case STATEMENT_INSERT:
                        printf("This is where we would do an insert.\n");
                        break;
                case STATEMENT_SELECT:
                        printf("This is where we would do a select.\n");
                        break;
        }
}
