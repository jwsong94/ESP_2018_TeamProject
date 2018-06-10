#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>
#include "face_rec_driver.h"
#include "socket_communication.h"

#define TRIG_PINNO 16
#define ECHO_PINNO 6
#define LED_R 22
#define LED_G 23
#define LED_B 24
#define SERVO 12

#define DOOR_OPEN 1500
#define DOOR_CLOSE 1000

#define TEST_FILE "capture.jpg"

void trigger(void);
void setLEDColor(unsigned int);
void cb_func_echo(int gpio, int level, uint32_t tick);
void copy_streaming_image(FILE *source);

extern pthread_mutex_t flag_lock;
extern int door_flag;

uint32_t start_tick_, dist_tick_;

unsigned int verify_count = 0;

int main(void)
{
    float distance;
    FILE *fp;

    gpioCfgClock(2, 1, 1);
    if (gpioInitialise()<0) return 1;

    init_socket_communication();
    printf("socket initialized\n");
    init_face_recognizer();
    printf("face_recognizer initialized\n");

    // Initialize Sonar Sensor
    gpioSetMode(TRIG_PINNO, PI_OUTPUT);
    gpioSetMode(ECHO_PINNO, PI_INPUT);

    // Initialize LED
    gpioSetMode(LED_R, PI_OUTPUT);
    gpioSetMode(LED_G, PI_OUTPUT);
    gpioSetMode(LED_B, PI_OUTPUT);

    // Initialize Servo
    gpioSetMode(SERVO, PI_OUTPUT); // Set PWM
    gpioSetPWMfrequency(SERVO, 400);
    gpioSetPWMrange(SERVO, 2500);

    gpioSetAlertFunc(ECHO_PINNO, cb_func_echo);
    gpioWrite(TRIG_PINNO, PI_OFF);
    gpioDelay(1000000);     // delay 1 second

    printf("Pi_Main Start\n");
    while(1){
        int df;
        read_door_flag(&df);
        if (df == FLAG_LOCK) {
            printf("closed\n");
            gpioServo(SERVO, DOOR_CLOSE);
            continue;
        }
        else if (df == FLAG_OPEN) {
            gpioServo(SERVO, DOOR_OPEN);
            continue;
        }

        setLEDColor(LED_B);
        gpioServo(SERVO, DOOR_CLOSE);
        start_tick_ = dist_tick_ = 0;
        gpioTrigger(TRIG_PINNO, 10, PI_HIGH);
        gpioDelay(50000);

        if(dist_tick_ && start_tick_){
            //distance = (float)(dist_tick_) / 58.8235;
            distance = dist_tick_ / 1000000. * 340 / 2 * 100;
            if(distance < 3 || distance > 400){
                printf("Sensor Error\n");
            }
            if(distance < 20){
                verify_count++;

                if(verify_count > 5){
                    verify_count = 0;
                    setLEDColor(LED_R);

                    struct log l;
                    // Python Code Call
                    memset(&l, 0, sizeof(l));

                    l.index = -1;
                    system("ffmpeg -i http://127.0.0.1:8081/ -vframes 1 capture.jpg");
                    l = face_recognition(TEST_FILE);
                    remove(TEST_FILE);

                    if(l.index >= 0){
                        printf("Valid Person\n");
                        setLEDColor(LED_G);
                        gpioServo(SERVO, DOOR_OPEN);
                        gpioDelay(10000000); // delay 10 second
                    }
                    else{
                        printf("Invalid Person\n");
                        setLEDColor(LED_R);
                        gpioServo(SERVO, DOOR_CLOSE);
                        gpioDelay(10000000); // delay 10 secont
                    }
                }
            }
            else{
                verify_count = 0;
            }
            //printf("interval : %6dus, Distance : %6.1f cm\n", dist_tick_, distance);
        }

        gpioDelay(100000);
    }
    gpioTerminate();
    close_face_recognizer();
    close_socket_communication();

    return 0;
}

void setLEDColor(unsigned int led){
    gpioWrite(LED_R, 0);
    gpioWrite(LED_G, 0);
    gpioWrite(LED_B, 0);

    gpioWrite(led, 1);
}

void cb_func_echo(int gpio, int level, uint32_t tick)
{
    if(level == PI_HIGH)
        start_tick_ = tick;
    else if(level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}

void copy_streaming_image(FILE *source)
{
    FILE *fp_out;
    int ch;

    fp_out = fopen("capture.jpg", "w");
    rewind(source);
    while ((ch = getc(source)) != EOF)
        putc(ch, fp_out);

    fclose(fp_out);
}
