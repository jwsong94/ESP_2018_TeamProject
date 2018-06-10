#ifndef _SOCKET_H_

#define _SOCKET_H_

#include "face_rec_driver.h"

#define F_REGIST 1
#define F_DEFAULT (1 << 1)
#define F_LOCK (1 << 2) 
#define F_OPEN (1 << 3)
#define DELIMITER '#'
#define SYNC 0

int init_socket_communication(void);
int close_socket_communication(void);
void send_log(struct log *lp);
void read_door_flag(int *val);

static void change_door_flag(int flag);
static void sync_with_server(void);
static void *read_order(void *args);
static void save_image(int socket_fd);
static void insert_name(char *name);

#endif
