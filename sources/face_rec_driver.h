#ifndef _FACE_REC_H_

#define _FACE_REC_H_

#define TIME_LEN 30
#define NAME_LEN 50

struct log {   
    int index; 
    char access_time[TIME_LEN];
    char name[NAME_LEN+1];
};

int init_face_recognizer(void);
int close_face_recognizer(void);
struct log face_recognition(char *path);

#endif
