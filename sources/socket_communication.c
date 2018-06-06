#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "socket_communication.h"
#include "face_rec_driver.h"

#define CLIENT_IP "10.27.0.166"
#define CLIENT_PORT 11111

static int client_socket;
static pthread_t th_read;

int init_socket_commuication(void)
{
    struct sockaddr_in server_address;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "open socket error on %d\n", __func__);
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CLIENT_PORT);
    server_address.sin_addr.s_addr = inet_addr(CLIENT_IP);

    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "connet error on %s\n", __func__);
        return -1;
    }

    pthread_create(&th_read, NULL, read_order, NULL);

    return 0;
}

int close_socket_communication(void)
{
    pthread_cancel(th_read);
    close(client_socket);

    return 0;
}


void send_log(struct log *lp)
{
    char buf[NAME_LEN+TIME_LEN+5];
    size_t time_len, name_len, total_len;

    memset(buf, 0, sizeof(buf));
    sprintf(buf + 1, "%s", lp->access_time);
    time_len = strlen(lp->access_time);
    buf[2+time_len] = DELIMITER;

    if (lp->index == -1)
        buf[0] = 0;
    else  {
        buf[0] = 1;
        sprintf(buf + 1 + time_len, "%s", lp->name);
        name_len = strlen(lp->name);
        buf[2+time_len+1+name_len] = DELIMITER;
    }
    buf[1] = DELIMITER;

    total_len = 2 + TIME_LEN + 1 + NAME_LEN + 1;
    write(client_socket, buf, total_len);
}

void *read_order(void *args)
{
}

