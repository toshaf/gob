#include <stdio.h>
#include <string.h>

void do_work() {
    char msg[7];
    //const char *smile = ":D";
    
    memcpy(msg, "Hiya", 4);
    msg[4] = ' ';
    memcpy(msg + 4, ":D", 2);

    printf("%s\n", msg);
}

int main(int argc, char** argv) {
    printf("in main\n");

    do_work();

    return 0;
}
