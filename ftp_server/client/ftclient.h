#ifndef FTP_SERV_FTCLIENT_H
#define FTP_SERV_FTCLIENT_H

#include "../common/common.h"

/**
 * Receive a response from server
 * Returns -1 on error, return code on success
 *
 * @return
 */
int read_reply();


/**
 * Print response message
 *
 * @param rc
 */
void print_reply(int rc);


/**
 * Parse command in cstruct
 *
 * @param buf
 * @param size
 * @param cstruct
 * @return
 */
int ftclient_read_command(char * buf, int size, struct command *cstruct);


/**
 * Do get <filename> command
 * @param data_sock
 * @param sock_control
 * @param arg
 * @return
 */
int ftclient_get(int data_sock, int sock_control, char * arg);


/**
 * Open data connection
 *
 * @param sock_con
 * @return
 */
int ftclient_open_conn(int sock_con);


/**
 * Do list command
 *
 * @param sock_data
 * @param sock_conn
 * @return
 */
int ftclient_list(int sock_data, int sock_conn);


/**
 * Input: cmd struct with an a code and an arg
 * Concats code + arg into a string and sends to server
 *
 * @param cmd
 * @return
 */
int ftclient_send_cmd(struct command * cmd);


/**
 * Get login details from user and send to server for authentication
 */
void ftclient_login();

#endif //FTP_SERV_FTCLIENT_H
