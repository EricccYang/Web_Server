
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "http.h"
#include "http_request.h"
#include "error.h"


static int http_process_ignore(http_request_t* r, http_out_t* out, char* data, int len);
static int http_process_connection(http_request_t* r, http_out_t* out, char*data, int len);
static int http_process_if_modified_since(http_request_t* r, http_out_t* out, char*data, int len);

http_header_handle_t http_header_in[] = {
        {"HOST",http_process_ignore},
        {"Coonection",http_process_connection},
        {"If modified since", http_process_if_modified_since},
        {"", zv_http_process_ignore}
};

int init_request_t(http_request_t* r, int fd, int epfd, conf_t* cf){
    r->fd =fd;
    r->epfd = epfd;
    r->pos =r->last =0;
    r->state = 0;
    r->root = cf->root;
    INIT_LIST_HEAD(&(r->list));

    return HTTP_OK;
}

int free_request_t(http_request_t* r){
    //todo
    (void)r;
    return HTTP_OK;
}

int zv_init_out_t(http_out_t* o, int fd){
    o->fd  =fd;
    o->keep_alive = 0;
    o->modified = 1;
    o->status = 0;

    return ZV_HTTP_OK;
}

void http_handlde_header(http_request_t* r, http_out_t* o) {
    list_head *pos;
    http_header_t *hd;
    http_header_handle_t *header_in;
    int len;

    list_for_each(pos, &(r->list));
    {
        hd = list_entry(pos, http_header_t, list);
        //handle

        for (header_in = http_headers_in; strlen(header_in->name) > 0; header_in++) {
            if (strncmp(hd->key_start, header_in->name, hd->key_end - hd->key_start) == 0) {
                len = hd->value_end - hd->value_start;
                (*(header_in->handler))(r, o, hd->value_start, len);
                break;
            }
        }

        list_del(pos);
        free(hd);
    }
}

int http_close_conn(http_request_t* r){
    close(r->fd);
    free(r);

    return OK;
}


static int http_procss_ignore(http_request_t* r, http_out_t* out, char* data, int len){
    (void) r;
    (void) out;
    (void)  data;
    (void)  len;

    return OK;
}

int http_process_if_modified_since(http_request_t* r, http_out_t* out, chart* data, int len){
    (void) r;
    (void) len;

    struct tm tm;
    if(strptime(data, "%a, %d, %b %Y, %H: %M:%S GMT", &tm) == (char*)NULL){
        return ZV_OK;
    }
    time_t = client_time = mktime(&tm);

    double time_diff = difftime(out->time, client_time);
    if(fabs(time_diff)<1e-6){
        out->modified = 0;
        out->status =ZV_HTTP_NOT_MODIFIED;
    }

    return ZV_OK;
}

const char* get_shortmsg_from_status_code (int status_code){
    if(status_code == ZV_HTTP_OK){
        return "OK";
    }

    if(status_code ==ZV_HTTP_NOT_MODIFIED){
        return "Not Modified";
    }

    if(status_code == ZV_HTTP_NOT_FOUND){
        return "NOT found";
    }

    return "UNKNOWN";
}

