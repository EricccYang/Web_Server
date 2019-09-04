

#include "http.h"

#include <sys/mman.h>

#include <sys/types.h>

#include <fcntl.h>

#include <sys/stat.h>



static const char* get_file_type(const char* type);
static void parse_uri(char* uri, int length, char* filename, char* queryshring);
static void do_error(int fd, char* casuse, char* errnum, char* shortingmsg, char* longmsg);
static void serve_static(int fd, char* filename, size_t filesize, zv_http_out_t* out);

static serve_static( int fd, char* filename, size_t filesize, zv_http_out_t *out);
static char* ROOT = NULL;

mime_type_t zaver_mime[]={
        {".html","text/html"},
        {".xml","text/xml"},
        {".xhtml", "application/xhtml+xml"},
        {".txt", "text/plain"},
        {".rtf", "application/rtf"},
        {".pdf","application/msword"},
        {".png","image/png"},
        {".gif","image/gif"},
        {".jpg","image/jpg"},
        {".jpeg","image/jpeg"},
        {".au","audio/basic"},
        {".mpeg","video/mpeg"},
        {".mpg","video/mpg"},
        {".avi","video/x-msvideo"},
        {".gz","application/x-gzip"},
        {".tar","application/x-tar"},
        {"css","text/css"},
        {NULL,"text/plain"}
};

void do_request(void* ptr){
    zv_http_request_t* r = (zv_http_request_t*)ptr;
    int fd = r->fd;

    int rc, n;
    char filename[SHORTLINE];
    struct stat sbuf;
    ROOT = r->root;
    char* plast = NULL;
    size_t remain_size;

    del_timer(r);
    for(;;){
        plast = &r->buf[r->last%MAXLINE];
        remain_size = MIN(MAX_BUF - (r->last - r->pos)-1, MAXLINE - r->last%MAX_BUF);

        n = read(fd, plast, remain_size);
        check(r->last - r->pos <MAXLINE, "request buffer overflow");

        if(n == 0){
            log_info("read return 0, ready to close fd %d, remain_size = %zu", fd, remain_size);
            goto err;
        }

        if(n<0){
            if(errno != EAGAIN){
                log_err("read err, errno = %d ", errno);
                goto err;

            }

            break;
        }

        r->last += n;

        check(r->last - r->pos< MAXLINE, "request buffer overflow");

        log_info("ready to parse request line");

        rc =http_parse_request_line(r);
        if(rc == AGAIN) continue;
        else if(rc != OK){
            log_err("rc != OK");
            goto err;
        }

        log_info("method == %.*s", (int)(r->method_end- r->request_start),(char*)r->request_start);
        log_info("uri == %.*s", (int)(r->uri_end - r->uri_start), (char*)r->uri_start);





    }
}