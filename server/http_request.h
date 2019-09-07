
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <time.h>
#include "http.h"
#include "list.h"

#define ZV_AGAIN EAGAIN

#define HTTP_PARSE_INVALID_METHOD       10
#define HTTP_PARSE_INVALID_REQUEST      11
#define HTTP_PARSE_INVALID_HEADER       12

#define HTTP_UNKNOWN        0x0001
#define HTTP_GET            0x0002
#define HTTP_HEAD           0x0004
#define HTTP_POST           0x0008

#define HTTP_OK             200

#define HTTP_NOT_MODIFIED   304
#define HTTP_NOT_FOUND      404

#define MAX_BUF             8124

typedef struct http_request_s{
    void** root;
    int fd;
    int epfd;
    char buf[MAX_BUF];
    size_t  pos,last;
    int state;
    void *request_start;
    void* method_end;
    int method;
    void*  uri_end;
    void*  uri_start;
    void*  path_start;
    void*  path_end;
    void*  query_end;
    void*  query_start;
    int http_major;
    int http_minor;
    void* request_end;


    struct list_head list;
    void* cur_header_key_start;
    void* cur_header_key_end;
    void* cur_header_value_start;
    void* cur_header_value_end;

    void* timer;
}http_request_t;


typedef struct{
    int fd;
    int keep_alive;
    time_t  mtime;
    int modified;
    int status;
}zv_http_out_t;

typedef struct http_header_s{
    void* key_start, *key_end;
    void* value_start, *value_end;
    list_head list;
}http_header_t;

typedef int (*http_header_handler_pt)(http_request_t* r, zv_http_out_t* o, char* data, int len);

typedef struct{
    char* name;
    zv_http_header_handler_pt handler;
}http_header_handle_t;

void http_handle_header(zv_http_request_t* r, http_out_t* o);
int  http_close_conn(http_request_t* r);

int http_init_request(http_request_t* r, int fd, int epfd, conf_t* cf);
int http_free_request(http_request_t* r);

int init_out_t(http_out_t* o, int fd);
int init_out_t(http_out_t* o);

const char* get_shortmsg_from_sta_code(int status_code);

extern http_header_handle_t  zv_http_headers_in[];

#endif

