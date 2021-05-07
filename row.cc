//
// Created by Wang Wei on 2021/5/7.
//

#include "row.h"

void serialize_row(Row *source, Row *destination)
{
        memcpy(destination, source, ROW_SIZE);
        // memcpy((void *)destination + ID_OFFSET, &(source->id), ID_SIZE);
        // memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
        // memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(Row *source, Row *destination)
{
        memcpy(destination, source, ROW_SIZE);
        // memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
        // memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
        // memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}