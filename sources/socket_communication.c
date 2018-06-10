#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "socket_communication.h"
#include "face_rec_driver.h"

#define CLIENT_IP "192.168.25.31"
#define CLIENT_PORT 11111 

static int client_socket;
static pthread_t th_read;
pthread_mutex_t flag_lock = PTHREAD_MUTEX_INITIALIZER;
int door_flag = FLAG_DEFAULT;

int init_socket_communication(void)
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
    sprintf(buf + 2, "%s", lp->access_time);
    time_len = strlen(lp->access_time);
    buf[2+time_len] = DELIMITER;

    if (lp->index == -1)
        buf[0] = 0;
    else  {
        buf[0] = 1;
        sprintf(buf + 3 + time_len, "%s", lp->name);
        name_len = strlen(lp->name);
        buf[2+time_len+1+name_len] = DELIMITER;
    }
    buf[1] = DELIMITER;

    total_len = 2 + TIME_LEN + 1 + NAME_LEN + 1;
    write(client_socket, buf, total_len);
}

static void sync_with_server(void)
{
    char ch;

    ch = SYNC;
    write(client_socket, &ch, sizeof(ch));
}

static void *read_order(void *args)
{
    char name[NAME_LEN+1];
    char buf[1024];
    char flag;
    int len;

    for (;;) {
        flag = 0;
        if ((len = recv(client_socket, &buf, sizeof(buf), 0)) == 0)
            continue;
        flag = buf[0] - '0';
        
        if (flag == FLAG_REGIST)  {
            sync_with_server();
            memset(name, 0, sizeof(name));
            read(client_socket, name, sizeof(name));
            sync_with_server();
            insert_name(name);
            save_image();
        }
        else if (flag == FLAG_DEFAULT) {
            change_door_flag(FLAG_DEFAULT);
        }
        else if (flag == FLAG_LOCK) {
            change_door_flag(FLAG_LOCK);
        }
        else if (flag == FLAG_OPEN) {
            change_door_flag(FLAG_OPEN);
        }
    }
}

static int save_image(void)
{
    FILE *fp_name, *fp_image;
    struct header h;
    char file_name[100] = {0};
    char buf[1024];

    if ((fp_name = fopen("./name/name_list", "r")) == NULL) {
        fprintf(stderr, "fopen error on %s\n", __func__);
        return -1;
    }

    fread(&h, sizeof(h), 1, fp_name); 
    sprintf(file_name, "./image/%d", h.num_record);

     if ((fp_image = fopen("file_name", "w")) == NULL) {
        fprintf(stderr, "fopen error on %s\n", __func__);
        return -1;
     }

    while (read(client_socket, buf, sizeof(buf)) != 0) 
        fwrite(buf, sizeof(buf), 1, fp_image);

    return 0;
}
    
static int insert_name(char *name)
{
    FILE *fp_name;
    struct header h;
    int ret;
    
    if ((fp_name = fopen("./name/name_list", "r+")) == NULL) {
        fprintf(stderr, "fopen error on %s\n", __func__);
        return -1;
    }

    if ((ret = fread(&h, sizeof(h), 1, fp_name) == 0)) {
        h.num_record = 0;
        h.list = -1;
    }

    fseek(fp_name, h.num_record * NAME_LEN, SEEK_SET);
    fwrite(name, NAME_LEN, 1, fp_name);
    fseek(fp_name, 0L, SEEK_SET);
    fwrite(&h, sizeof(h), 1, fp_name);
    
    return 0;
}

static void change_door_flag(int flag)
{
    pthread_mutex_lock(&flag_lock);
    door_flag = flag;
    pthread_mutex_unlock(&flag_lock);
}

void read_door_flag(int *val)
{
    pthread_mutex_lock(&flag_lock);
    *val = door_flag;
    pthread_mutex_unlock(&flag_lock);
}
