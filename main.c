#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "parser.h"
#include "exec.h"

void print_prompt()
{
        printf("db > ");
}

typedef enum
{
        META_COMMAND_SUCCESS,
        META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// #define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
//
// const uint32_t ID_SIZE         = size_of_attribute(Row, id);
// const uint32_t USERNAME_SIZE   = size_of_attribute(Row, username);
// const uint32_t EMAIL_SIZE      = size_of_attribute(Row, email);
// const uint32_t ID_OFFSET       = 0;
// const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
// const uint32_t EMAIL_OFFSET    = USERNAME_OFFSET + USERNAME_SIZE;
// const uint32_t ROW_SIZE        = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
//
// const uint32_t PAGE_SIZE = 4096;
// #define MAX_PAGES_PER_TABLE 100
// const uint32_t ROWS_PER_PAGE  = PAGE_SIZE / ROW_SIZE;
// const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * MAX_PAGES_PER_TABLE;

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

                // execute_statement(&statement);
                // printf("Executed.\n");
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