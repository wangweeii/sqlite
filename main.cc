#include <iostream>
#include "exec.h"
#include "input.h"
#include "table.h"
#include "parser.h"

void print_prompt()
{
        printf("db> ");
}

typedef enum
{
        META_COMMAND_SUCCESS,
        META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// 执行元命令
MetaCommandResult do_meta_command(InputBuffer *input_buffer)
{
        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
                exit(EXIT_SUCCESS);
        }
        else
        {
                return META_COMMAND_UNRECOGNIZED_COMMAND;
        }
}

int main()
{
        Table *table = new_table();
        InputBuffer *input_buffer = new_input_buffer();
        while (true)
        {
                print_prompt();
                read_input(input_buffer);

                if (input_buffer->buffer[0] == '.')
                {
                        switch (do_meta_command(input_buffer))
                        {
                                case (META_COMMAND_SUCCESS):
                                        continue;
                                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                                        printf("Unrecognized command '%s'\n", input_buffer->buffer);
                                        continue;
                        }
                }

                Statement statement;
                switch (prepare_statement(input_buffer, &statement))
                {
                        case (PREPARE_SUCCESS):
                                break;
                        case (PREPARE_SYNTAX_ERROR):
                                printf("Syntax error. Could not parse statement.\n");
                                continue;
                        case (PREPARE_UNRECOGNIZED_STATEMENT):
                                printf("Unrecognized keyword at start of '%s'\n", input_buffer->buffer);
                                continue;
                }

                switch (execute_statement(&statement, table))
                {
                        case EXECUTE_SUCCESS:
                                printf("Executed.\n");
                                break;
                        case EXECUTE_TABLE_FULL:
                                printf("Error: Table full.\n");
                                break;
                }
        }
        return 0;
}
