#ifndef _SOCKET_H_

#define _SOCKET_H_

#include "face_rec_driver.h"

#define FLAG_REGIST 1
#define FLAG_DEFAULT (1 << 1)
#define FLAG_LOCK (1 << 2) 
#define FLAG_OPEN (1 << 3)
#define DELIMITER '/'
#define SYNC 0

struct header {
    uint16_t num_record;
    uint16_t list;
};

int init_socket_communication(void);
int close_socket_communication(void);
void send_log(struct log *lp);
void read_door_flag(int *val);

static void change_door_flag(int flag);
static void sync_with_server(void);
static void *read_order(void *args);
static int save_image();
static int insert_name(char *name);

#endif
