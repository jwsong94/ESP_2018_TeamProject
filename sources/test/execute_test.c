#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Program Start\n");
    sleep(1);
    printf("Execute Start\n");
    sleep(1);
    system("./script.py");

    printf("Execute End\n");
    sleep(1);
    printf("Program End\n");
    sleep(1);

    return 1;
}
