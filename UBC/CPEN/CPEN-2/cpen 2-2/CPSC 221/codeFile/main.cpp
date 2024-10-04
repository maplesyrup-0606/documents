#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    fprintf(stdout,"hello ");
//    fflush(stdout);
    write(STDOUT_FILENO, "world", 5);
    return 0;
}