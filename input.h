//
// Created by w15627 on 04/25.
//

#ifndef SQLITE_INPUT_H
#define SQLITE_INPUT_H

#include <stdlib.h>

typedef struct
{
        char    *buffer;
        size_t  buffer_length;
        ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer();

void read_input(InputBuffer *input_buffer);

void close_input_buffer(InputBuffer *input_buffer);

#endif //SQLITE_INPUT_H
