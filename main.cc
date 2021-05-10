#include <iostream>
#include <cstring>
#include "exec.h"
#include "input.h"
#include "save.h"
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
MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table)
{
        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
                db_close(table);
                exit(EXIT_SUCCESS);
        }
        else
        {
                return META_COMMAND_UNRECOGNIZED_COMMAND;
        }
}

int main(int argc, char *argv[])
{
        // printf("%d %d %d\n", sizeof(int), sizeof(long), sizeof(long long));
        if (argc < 2)
        {
                printf("Must supply a database filename.\n");
                exit(EXIT_FAILURE);
        }

        std::string filename = argv[1];
        Table *table = db_open(filename.c_str());
        InputBuffer *input_buffer = new_input_buffer();
        while (true)
        {
                print_prompt();
                read_input(input_buffer);

                if (input_buffer->buffer[0] == '.')
                {
                        switch (do_meta_command(input_buffer, table))
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
                        case PREPARE_SUCCESS:
                                break;
                        case PREPARE_NEGATIVE_ID:
                                printf("ID must be positive.\n");
                                continue;
                        case PREPARE_SYNTAX_ERROR:
                                printf("Syntax error. Could not parse statement.\n");
                                continue;
                        case PREPARE_STRING_TOO_LONG:
                                printf("String is too long.\n");
                                continue;
                        case PREPARE_UNRECOGNIZED_STATEMENT:
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
