#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define TRIG_PINNO 16
#define ECHO_PINNO 6
#define LED_R 22
#define LED_G 23
#define LED_B 24

void trigger(void);
void setLEDColor(unsigned int);
void cb_func_echo(int gpio, int level, uint32_t tick);

uint32_t start_tick_, dist_tick_;

unsigned int verify_count = 0;

int main()
{
    float distance;
    gpioCfgClock(2, 1, 1);
    if (gpioInitialise()<0) return 1;

    // Initialize Sonar Sensor
    gpioSetMode(TRIG_PINNO, PI_OUTPUT);
    gpioSetMode(ECHO_PINNO, PI_INPUT);

    // Initialize LED
    gpioSetMode(LED_R, PI_OUTPUT);
    gpioSetMode(LED_G, PI_OUTPUT);
    gpioSetMode(LED_B, PI_OUTPUT);

    gpioSetAlertFunc(ECHO_PINNO, cb_func_echo);
    gpioWrite(TRIG_PINNO, PI_OFF);
    gpioDelay(1000000);     // delay 1 second

    printf("Pi_Main Start\n");

    while(1){
        setLEDColor(LED_B);
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
                    
                    char c = getchar();
                    // Python Code Call

                    if(c == 'o'){
                        printf("Valid Person\n");
                        setLEDColor(LED_G);
                        gpioDelay(10000000); // delay 10 second
                    }
                    else{
                        printf("Invalid Person\n");
                        setLEDColor(LED_R);
                        gpioDelay(10000000); // delay 10 secont
                    }
                }
            }
            else{
                verify_count = 0;
            }
            printf("interval : %6dus, Distance : %6.1f cm\n", dist_tick_, distance);
        }

        gpioDelay(100000);
    }
    gpioTerminate();

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
