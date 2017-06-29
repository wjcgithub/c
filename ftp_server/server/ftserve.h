/* ftserve.h
 *
 *
 * Server side of TCP file transfer implementation, runs with custom client,
 * ftclient.c. Sends list of files in current directory and files to ftclient.
 * Requires user login.
 *
 * Usage:
 *    ./ftserve PORT#
 */

#ifndef FTP_SERV_FTSERVE_H
#define FTP_SERV_FTSERVE_H

#include "../common/common.h"


/**
 * Send file specified in filename over data connection, sending control message over control connection
 * Handles case of null or invalid filename
 *
 * @param sock_control
 * @param sock_data
 * @param filename
 */
void ftserve_retr(int sock_controll, int sock_data, char * filename);


/**
 * Send list of files in current directory
 * over data connection
 * Return -1 on error, 0 on success
 *
 * @param sock_dta
 * @param sock_controll
 * @return
 */
int ftserve_list(int sock_dta, int sock_controll);


/**
 * Opend data connection to client
 * Returns socket for data connection
 * or -1 on error
 *
 * @param sock_controll
 * @return
 */
int ftserve_start_data_conn(int sock_controll);


/**
 * Authenticate a user's credentials
 * Return 1 if authenticated, 0 if not
 *
 * @param user
 * @param pass
 * @return
 */
int ftserve_check_user(char * user, char * pass);


/**
 * Login  connected client
 *
 * @param sock_controll
 * @return
 */
int ftserve_login(int sock_controll);


/**
 * Wait for command from client and send response
 * Returns response code
 *
 * @param sock_controll
 * @param cmd
 * @param arg
 * @return
 */
int ftserve_recv_cmd(int sock_controll, char * cmd, char * arg);


/**
 * Child process handles connection to client
 *
 * @param sock_controll
 */
void ftserve_process(int sock_controll);

#endif //FTP_SERV_FTSERVE_H
