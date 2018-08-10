#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
int lock = 0;
void spinlock() {
    lock = !lock;
}
int main(int ac, char* argv[])
{
    struct termios old_settings, new_settings;
    tcgetattr(0, &old_settings);
    tcgetattr(0, &new_settings);
    int countdown = atoi(argv[1]);
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~IXANY;
    tcsetattr(0, TCSANOW, &new_settings);
    signal(SIGINT, spinlock);
    while (countdown >= 0) {
        printf("%d\n", countdown);
        sleep(1);
        while (lock) {}
        countdown--;
    }
    tcsetattr(0, TCSANOW, &old_settings);
    exit(0);
}