

#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#define CR '\r'
#define LF '\n'

#define CRLFCRLF "\r\n\r\n"
#include "http_request.h"

int parse_request_line(http_request_t* r);
int parse_request_body(http_request_t* r);

#endif
