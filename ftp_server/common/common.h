//
// Created by evolution on 17-6-27.
//

#ifndef FTP_SERV_COMMON_H
#define FTP_SERV_COMMON_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * constants
 */
#define DEBUG             1
#define MAXSIZE           512   //MAX buffer size
#define CLIENT_PORT_ID    30020

/**
 * Holds command code and argument
 */
struct command {
    char arg[255];
    char code[5];
};


/**
 * Create listening socket on remote host
 * Returns -1 on error, socket fd onsucess
 */
int socket_create(int port);


/**
 * Create new socket for incoming client connection request
 * Returns -1 on error, or fd of newly created socket
 */
int socket_accept(int sock_listen);


/**
 * Connect to remote host at given port
 * Returns socket fd onsuccess, -1 on error
 */

int socket_connect(int port, char *host);


/**
 * Receive data on sockfd
 * Returns -1 on error, number of bytes received
 * on success
 */
int recv_data(int sockfd, char * buf, int bufsize);


/**
 * Send response code on sockfd
 * Returns -1 on error, 0 on success
 *
 */
int send_response(int sockfd, int rc);


//-------------UTILITY FUNCTIONS------------------------//

/**
 * Trim whitespace and line ending
 * characters from a string
 */
void trimstr(char *str, int n);


/**
 * Read input from command line
 */
void read_input(char * buffer, int size);

#endif //FTP_SERV_COMMON_H
