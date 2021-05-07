//
// Created by Wang Wei on 2021/5/7.
//

#include "row.h"

void serialize_row(Row *source, Row *destination)
{
        memcpy(destination, source, sizeof(Row));
}

void deserialize_row(Row *source, Row *destination)
{
        memcpy(destination, source, sizeof(Row));
}