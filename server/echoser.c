






//simple TCP/IP server

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <helper.h>

#include <stdlib.h>
#include <stdio.h>

//global contants

#define ECHO_PORT (2002)
#define MAXLINE   (1000)

int main(int argc, char* argv[]){
    int list_s;
    int conn_s;
    short int port;
    struct socaddr_in seradd;
    char buffer[MAXLINE];
    char* endptr;               //for strtol;

    if( argc == 2){
        port = strtol(argv[1], &endptr, 0);     //string to long int ,代表用几进制转乘10进制
        if(*endptr){
            fprintf(stderr, "ECHOSERV : Invalid port number.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(argc<2){
        port = ECHO_PORT;
    }
    else{
        fprintf(stderr,"error");
        exit(EXIT_FAILURE);
    }

    //create listening socket
    if(list_s = socket(AF_INET, SOCK_STREAM,0)<0){  //create a socket, 协议；类型，某个协议的特定类型
        fprintf(stderr, "error");
        exit();
    }


    //set all bytes in socket address structure to zero

    memset(&seradd, 0, sizeof(seradd));
    seradd.sin_fmaliy   = AF_INET;
    seradd.sin_addr.s_addr = htonl(INADDR_ANY);////////
    seradd.sin_port         = htons(port);                          //////////

    //bind the socket address to listening socket and call listen
    if(bind(list_s,(struct socaddr_in*)&seradd, sizeof(seradd))<0){
        fprintf(stderr,"error");
        exit();
    }

    if(listen(list_s,LISTENQ)<0){  //最大排列数字
        fprintf(stderr,"error");   //把套接字转换成监听字
        exit();
    }

    while(1){
        //wait for a connection
        if(conn_s = accept(list_s, NULL, NULL)<0){
            fprintf(stderr,"error");
            exit();
        }

        Readline;
        Writeline;

        if(close(conns)<0){
            fprintf();
            exit();
        }
    }




}