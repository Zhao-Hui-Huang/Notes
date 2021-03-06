//
// Created by hui on 19-4-10.
//

#ifndef EPOLLLEARN_SERVER_H
#define EPOLLLEARN_SERVER_H

#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <climits>

extern "C" {
#include <csapp/csapp.h>
}

#define MAX_EVENTS 10
epoll_event ev, events[MAX_EVENTS];
int nfds, epollfd;

struct TP {
    char *addr;
    int port;
};

/**
 * 监听端口，对于每个连接的客户端，第一次连接时，向客户端发送hello，以及当前时间，
 * 以后对于客户端的每次发送的内容，返回客户端的发送内容以及当前时间。
 * 将每次收到内容以及发送的内容打印在控制台上。
 * 当收到控制台传来的中断信号时，断开所有打开的连接并关闭子进程
 * 使用epoll
 */
class Server {
private:
    static const int MAX_LENGTH = 1024;

public:
    void Run() {
        char listen_port[] = "8080";
        int listen_fd, connfd;
        int maxi, nready;
        std::map<int, TP> map;
//        pollfd clients[1024];

        socklen_t clientlen;
        sockaddr_storage client_addr;

        listen_fd = Open_listenfd(listen_port);
//
//        FD_ZERO(&read_set);
//        FD_SET(listen_fd, &read_set);

//        clients[0].fd = listen_fd;
//        clients[0].events = POLLIN;
//        for (int i = 1; i < 1024; i++) {
//            clients[i].fd = -1;
//        }
//        maxi = 0;

        epollfd = epoll_create1(0);
        if (epollfd == -1) {
            perror("epoll_create1");
            exit(EXIT_FAILURE);
        }

        ev.events = EPOLLIN;
        ev.data.fd = listen_fd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
            perror("epoll_ctl: listen_sock");
            exit(EXIT_FAILURE);
        }

        connfd = listen_fd;
        while (true) {

//            nready = poll(clients, maxi + 1, INFINITY);
            nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
            if (nfds == -1) {
                perror("epoll_wait");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < nfds; ++i) {
                // 监听的端口
                if (events[i].data.fd == listen_fd) {
                    clientlen = sizeof(sockaddr_storage);
                    connfd = Accept(listen_fd, (SA *) &client_addr, &clientlen);

                    // TODO 1024
                    ev.events = EPOLLIN;
                    ev.data.fd = connfd;
                    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd,
                                  &ev) == -1) {
                        perror("epoll_ctl: conn_sock");
                        exit(EXIT_FAILURE);
                    }

//                    for (int i = 1; i < 1024; i++) {
//                        if (clients[i].fd < 0) {
//                            clients[i].fd = connfd;
//                            clients[i].events = POLLIN;
//                            if (i > maxi) {
//                                maxi = i;
//                            }
//                            break;
//                        }
//                    }

                    sockaddr_in *sockaddrIn = (sockaddr_in *) &client_addr;
                    char *cur_client_addr = inet_ntoa(sockaddrIn->sin_addr);
                    int port = ntohs(sockaddrIn->sin_port);

                    map[connfd] = {cur_client_addr, port};

                    // 打印连接的客户端
                    printf("%s:%d connect\n", cur_client_addr, port);

                    // 主动发送 "hello"
                    const char *HELLO_STR = "hello";
                    if (send(connfd, HELLO_STR, strlen(HELLO_STR), 0) < 0) {
                        fprintf(stderr, "%s: %s\n", "child process", strerror(errno));
                        exit(3);
                    }


                } else {
                    auto cur_conn_fd = events[i].data.fd;
                    auto tp = map[cur_conn_fd];

                    char buf[MAX_LENGTH];
                    char *cur_client_addr = tp.addr;
                    int cur_client_port = tp.port;

                    // TODO 使用while读取多段数据
                    // 读取客户端的数据
                    int length = recv(cur_conn_fd, buf, MAX_LENGTH, 0);
                    if (length < 0) {
                        // 发生错误
                        Close(cur_conn_fd);
                        fprintf(stderr, "%s: %s\n", "recv", strerror(errno));
                        exit(3);
                    } else if (0 == length) {
                        // 客户端断开连接
                        Close(cur_conn_fd);
                        printf("%s:%d disconnected\n", cur_client_addr, cur_client_port);
                        map.erase(cur_conn_fd);
                    } else {
                        // 将读取到的数据输出到控制台
                        printf("recv(%s:%d): %s\n", cur_client_addr, cur_client_port, buf);

                        // 将数据返回给客户端
                        if (send(cur_conn_fd, buf, strlen(buf), 0) < 0) {
                            fprintf(stderr, "%s: %s\n", "send", strerror(errno));
                            exit(3);
                        }
                    }
                }
            }
        }
    }
};

#endif // EPOLLLEARN_SERVER_H
