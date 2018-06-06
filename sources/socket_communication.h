#ifndef _SOCKET_H_

#define _SOCKET_H_

#include "face_rec_driver.h"

#define DELIMITER '#'

int init_socket_communication(void);
int close_socket_communication(void);
void send_log(struct log *lp);
void *read_order(void *args);

#endif
