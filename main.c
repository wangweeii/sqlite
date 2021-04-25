#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"

void print_prompt()
{
        printf("db > ");
}

typedef enum
{
        EXECUTE_SUCCESS,
        EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum
{
        META_COMMAND_SUCCESS,
        META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum
{
        PREPARE_SUCCESS,
        PREPARE_SYNTAX_ERROR,
        PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum
{
        STATEMENT_INSERT,
        STATEMENT_SELECT
} StatementType;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct
{
        uint32_t id;
        char     username[COLUMN_USERNAME_SIZE];
        char     email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct
{
        StatementType type;
        Row           row_to_insert;
} Statement;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32_t ID_SIZE         = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE   = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE      = size_of_attribute(Row, email);
const uint32_t ID_OFFSET       = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET    = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE        = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const uint32_t ROWS_PER_PAGE  = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct
{
        uint32_t num_rows;
        void     *pages[TABLE_MAX_PAGES];
}              Table;

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

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
        if (strncmp(input_buffer->buffer, "insert", 6) == 0)
        {
                statement->type   = STATEMENT_INSERT;
                int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
                                           statement->row_to_insert.username, statement->row_to_insert.email);
                if (args_assigned < 3)
                {
                        return PREPARE_SYNTAX_ERROR;
                }
                return PREPARE_SUCCESS;
        }
        if (strcmp(input_buffer->buffer, "select") == 0)
        {
                statement->type = STATEMENT_SELECT;
                return PREPARE_SUCCESS;
        }

        return PREPARE_UNRECOGNIZED_STATEMENT;
}

void serialize_row(Row *source, void *destination)
{
        memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
        memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
        memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void *source, Row *destination)
{
        memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
        memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
        memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

// 一行不能存放到不同的页上
void *row_slot(Table *table, uint32_t row_num)
{
        uint32_t page_num = row_num / ROWS_PER_PAGE;
        void     *page    = table->pages[page_num];

        if (page == NULL)
        {
                page = table->pages[page_num] = malloc(PAGE_SIZE);
        }

        uint32_t row_offset  = row_num % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;

        return page + byte_offset;
}

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
        Row           row;
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
                case (STATEMENT_INSERT):
                        // printf("This is where we would do an insert.\n");
                        // break;
                        return execute_insert(statement, table);
                case (STATEMENT_SELECT):
                        // printf("This is where we would do a select.\n");
                        // break;
                        return execute_select(statement, table);
        }
}

int main()
{
        InputBuffer *input_buffer = new_input_buffer();
        while (true)
        {
                print_prompt();
                read_input(input_buffer);

                // if (strcmp(input_buffer->buffer, ".exit") == 0)
                // {
                // close_input_buffer(input_buffer);
                // exit(EXIT_SUCCESS);
                // }
                // else
                // {
                //         printf("Unrecognized command '%s'.\n", input_buffer->buffer);
                // }
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
                        case (PREPARE_UNRECOGNIZED_STATEMENT):
                                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                                continue;
                }
                execute_statement(&statement);
                printf("Executed.\n");
        }

        return 0;
}