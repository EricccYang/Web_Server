/* A high performance Web Server*/
/* Kaicheng Yang*/

#include <stdint.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include "util.h"
#include "timer.h"
#include "epoll.h"
#include "threadpool.h"


#define  CONF "server_ky.conf"
#define  PROGRAM_VERSION "0.1"

extern struct epoll_event* events;

static const struct option long_options[]={
        {"help",no_argument,NULL,'?'},
        {"version",no_argument,NULL,'V'},
        {"conf",required_argument,NULL,'c'},
        {NULL,0,NULL,0}
};

static void usage(){
    fprintf(stderr,
    "zaver [option]...\n"
    " -c|--conf<config file>    Specify config file. Default ./server_ky.conf.\n"
    " -?|-h|--help              This information.\n"
    " -V|--version              Display program version.\n"
    );
}

int main (int argc, char* argv[]){
    int rc;
    int opt =0;
    int option_index =0;
    char* conf_file = CONF;

    /* parse argv*/

    if(argc==1){
        usage();
        return 0;
    }

    while(( opt=getopt_long( argc,argv,"Vc:?h", long_options, &option_index))!=EOF){
        switch (opt){
            case 0 : break;
            case 'c':
                conf_file = optarg;
                break;
            case 'V':
                printf(PROGRAM_VERSION "\n");
                return 0;
            case ':':
            case 'h':
            case '?':
                usage();
                return 0;
        }
    }

    debug("conffile = %s" , conf_file);
    if( optind < argc ){
        log_err("non-option ARGV-elements:  ");
        while (optind < argc )
            log_err("%s", argv[optind++]);
        return 0;
    }


    /* read config file */
    char conf_buf[BUFLEN];
    zv_conf_t  cf;
    rc = read_conf(conf_file, &cf ,conf_buf, BUFLEN);
    check( rc == CONF_OK, "read conf err ");


    //install signal handle for SIGPIPE,(terminate the processs)
    struct sigaction sa;
    memset (&sa, '\0', sizeof(sa));
    sa.sa_hander = SIG_IGN;
    sa.sa_flag =0;
    if(sigaction(SIGPIPE,&sa,NULL)){
        log_err("install sigal hander for SIGPIPE failed");
        return 0;
    }


    //initialize listening socket
    int listenfd;
    struct sockaddr_in clientaddr;
    socklen_t inlen = 1;
    memset(&clientaddr, 0 ,sizeof(struct sockaddr_in));

    listenfd = open_listenfd(cf.port);
    rc = make_socket_non_blocking(listenfd);     //set the descripter non_blocking
    check(rc == 0, "make_socket_non_blocking");

    //create epoll and add listenfd to ep
    int epfd = epoll_create(0);
    struct epoll_event event;

    http_request_t* request = (http_request_t*)malloc(sizeof(http_request_t));
    init_requset_t(request, listenfd, epfd, &cf);   //bind the listen port，epoll descripter and request

    event.data.ptr = (void*)request;
    event.events = EPOLLIN|EPOLLET;                     //read & edge trigger
    epoll_add(epfd,listenfd, &event);

    //creat thread pool
    threadpool_t *tp = threadpool_init(cf.thread_num);   //starting multi threads
    check(tp!= NULL, "threadpool_init error");

    //initialize timer
    timer_init();

    log_info("server started.");
    int n;
    int i, fd;
    int time;

    while(1){                                           //one single epoll event per while loop
        time = find_timer();
        debug("wait time = %d", time);
        n = epoll_wait(epfd, events, MAXEVENTS, time);   //how many events ready
        handle_expire_times();

        for(i = 0;i<n;i++){
            http_request_t* r = (http_request_t*)events[i].data.ptr;
            fd = r->fd;

            if(listenfd == fd){

                int infd;
                while(1){
                    infd = accept(listenfd, (struct sockaddr*)&clientaddr, &inlen);
                    if(infd<0){
                        if((errno == EAGAIN)||(errno == EWOULDBLOCK)){
                            //process all incoming connections
                            break;
                        }
                        else{
                            log_err("accept");
                            break;
                        }
                    }

                    rc = make_socket_non_blocking(infd);
                    check(rc == 0, "make_socket_non_blocking");
                    log_info("new connection fd %d", infd);

                    http_request_t* request =(http_request*)malloc(sizeof(http_request_t));
                    if(request == NULL){
                        log_err("malloc(sizeof(http_request_t))");
                        break;
                    }

                    init_request_t(request, infd, epfd, &cf);
                    event.data.ptr = (void*) request;
                    event.events = EPOLLIN|EPOLLET|EPOLLONESHOT;

                    epoll_add(epfd, infd, &event);
                    add_timer(request, TIMEOUT_DEFAULT, http_close_cond);

                }    //end of while loop
            }
            else{
                if((events[i].events & EPOLLERR)||(events[i].events & EPOLLERR)||(!(events[i].events & EPOLLIN))){
                    log_err("epoll_err fd :%d", r->fd);
                    close(fd);
                    continue;
                }
                log_info("new data from fd %d",fd);

                do_request(events[i],data,ptr);
            }

        } //end of for(n) loop (n : epoll_wait result)

    } //while loop, continue serve after one single epoll event


}  //end of whole process

