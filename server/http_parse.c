
#include "http_parse.h"
#include "http.h"
#include "debug.h"

int parse_request_line(http_request_t* r){
    u_char ch, *p, *m;
    size_t pi;

    enum{
        sw_start = 0,
        sw_method,
        sw_space_before_uri,
        sw_after_slash_in_uri,
        sw_http,
        sw_http_H,
        sw_http_HT,
        sw_http_HTT,
        sw_http_HTTP,
        sw_first_major_digit,
        sw_major_digit,

    }state;

    state= r->state;

    for(pi=r->pos;pi<r->last;pi++){
        p = (u_char*)(&r->buf[pi % MAX_BUF]);


        switch(state){
            case sw_start:
                r->request_start = p;

                if(ch == CR || ch == LF) break;
                if((ch<'A'||ch>'Z')&&ch!='_')   return HTTP_PARSE_INVALID_METHOD;

                state = sw_method;
                break;

            case sw_method:
                if(ch == ' '){
                    r->method_end = p;
                    m = r->method_start;

                    switch(p-m){
                        case 3:
                            if(zv_str3_cmp(m, 'G', 'E','T',' ')){
                                r->method = HTTP_GET;
                            }
                            break;
                        case 4:
                            if(zv_str30cmp(m, 'P','O','S','T')){
                                r->method = HTTP_POST;
                                break;
                            }

                            if(zv_str4cmp(m,'H','E','A',"D")){
                                r->method = HTTP_HEAD;
                            }
                            break;
                        default:
                            r->method = HTTP_UNKNOWN;
                            break;
                    }
                    state = sw_spaces_before=_uri;
                    break;
                }

                if(ch == CR || ch == LF) break;
                if((ch<'A'||ch>'Z')&&ch!='_')   return INVALID_METHOD;

                break;

            case sw_space_before_uri:

                if(ch =='/'){
                    r->uri_start = p;
                    state = sw_after_slash_in_uri;
                    break;
                }

                switch(ch){
                    case ' ':
                        break;
                    default:
                        return INVALID_REQUEST;
                }
                break;

            case sw_after_slash_in_uri:
                switch(ch){
                    case ' ':
                        r->uri_end = p;
                        state = sw_http;
                        break;
                    default:
                        break;
                }
                break;

            case sw_http:
                switch (ch){
                    case ' ':
                        break;
                    case 'H':
                        state = sw_http_H;
                        break;
                    default:
                        return INVALID_REQUEST;
                }
                break;
            case sw_http:
                switch(ch){
                    case ' ':
                        break;
                    case 'H':
                        state = sw_http_H;
                        break;
                    default:
                        return HTTP_PARSE_INVALID;

                }
                break;
            case sw_http_H:
                switch(ch){
                    case 'T':
                        state = sw_http_HTT;
                        break;
                    default:
                        return HTTP_PARSE_INVALID;
                }
                break;


        }
    }
}


int http_parse_request_body(http_request_t* r){
    u_char ch,*p;

    size_t pi;

    enum{
        sw_start = 0,
        sw_key,
        sw_spaces_before_colon,
        sw_spaces_after_colon,
        sw_value,
        sw_cr,
        sw_crlf,
        sw_lf,

    }state;

    state = r->state;
    check(state == 0,"state should be 0");

    http_header_t* hd;
    for(pi= r->pos;pi<r->last;pi++){
        p = (u_char*)&r->buf[pi%MAXLINE];
        ch= *pl;

        switch(state){
            case sw_start:
                if(ch == CR|| ch == LF){
                    break;
                }

                r->cur_header_key_start = p;
                state = sw_key;
                break;
            case sw_key:
                if (ch == ' ') {
                    r->cur_header_key_end = p;
                    state = sw_spaces_before_colon;
                    break;
                }

                if (ch == ':') {
                    r->cur_header_key_end = p;
                    state = sw_spaces_after_colon;
                    break;
                }

                break;
            case sw_spaces_before_colon:
                if (ch == ' ') {
                    break;
                } else if (ch == ':') {
                    state = sw_spaces_after_colon;
                    break;
                } else {
                    return ZV_HTTP_PARSE_INVALID_HEADER;
                }
            case sw_spaces_after_colon:
                if (ch == ' ') {
                    break;
                }

                state = sw_value;
                r->cur_header_value_start = p;
                break;
            case sw_value:
                if (ch == CR) {
                    r->cur_header_value_end = p;
                    state = sw_cr;
                }

                if (ch == LF) {
                    r->cur_header_value_end = p;
                    state = sw_crlf;
                }

                break;
            case sw_cr:
                if (ch == LF) {
                    state = sw_crlf;
                    // save the current http header
                    hd = (zv_http_header_t *)malloc(sizeof(zv_http_header_t));
                    hd->key_start   = r->cur_header_key_start;
                    hd->key_end     = r->cur_header_key_end;
                    hd->value_start = r->cur_header_value_start;
                    hd->value_end   = r->cur_header_value_end;

                    list_add(&(hd->list), &(r->list));

                    break;
                } else {
                    return ZV_HTTP_PARSE_INVALID_HEADER;
                }

            case sw_crlf:
                if (ch == CR) {
                    state = sw_crlfcr;
                } else {
                    r->cur_header_key_start = p;
                    state = sw_key;
                }
                break;

            case sw_crlfcr:
                switch (ch) {
                    case LF:
                        goto done;
                    default:
                        return ZV_HTTP_PARSE_INVALID_HEADER;
                }
                break;
        }
    }

    r->pos = pi;
    r->state = state;

    return ZV_AGAIN;

    done:
    r->pos = pi + 1;

    r->state = sw_start;

    return ZV_OK;
}