//
// Created by Wang Wei on 2021/5/7.
//

#ifndef SQLITE___INPUT_H
#define SQLITE___INPUT_H

#include <cstdio>

struct InputBuffer
{
        char *buffer;
        size_t buffer_length;
        ssize_t input_length;
};

InputBuffer *new_input_buffer();

void read_input(InputBuffer *input_buffer);

void close_input_buffer(InputBuffer *input_buffer);

#endif //SQLITE___INPUT_H
