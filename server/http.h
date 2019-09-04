

#ifndef HTTP_H
#define HTTP_H

#include <strings.h>
#include <stdint.h>
#include "read.h"






#define MAXLINE 8192

#define SHORTLINE 512

#define zv



typedef struct mime_type_s{
    const char* type;
    const char* value;
}mime_type_t;


void do_request(void* infd);

#endif