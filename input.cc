//
// Created by Wang Wei on 2021/5/7.
//

#include <cstdlib>
#include "input.h"

InputBuffer *new_input_buffer()
{
        auto *inputBuffer = (InputBuffer *) malloc(sizeof(InputBuffer));
        inputBuffer->buffer = nullptr;
        inputBuffer->buffer_length = 0;
        inputBuffer->input_length = 0;

        return inputBuffer;
}

void read_input(InputBuffer *input_buffer)
{
        ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
        if (bytes_read <= 0)
        {
                printf("Error reading input\n");
                exit(EXIT_FAILURE);
        }

        input_buffer->input_length = bytes_read - 1;
        input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer)
{
        free(input_buffer->buffer);
        free(input_buffer);
}