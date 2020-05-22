#define _GNU_SOURCE
#include<sched.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>


int  print_child() {
    printf("PID as seen from the child: %d\n", getpid());
    system("bash");
}

int main() {
    int namespaces = CLONE_NEWUTS|CLONE_NEWPID|CLONE_NEWIPC|CLONE_NEWNS|CLONE_NEWNET;
    pid_t p = clone(print_child, malloc(4096) +4096, SIGCHLD|namespaces, NULL);
    if ( p == -1 ) {
        perror("Clone: clould not clone the process.");
        exit(1);
    }
    printf("Child pid is: %d\n",p);
    waitpid(p,NULL,0);
    return 0;
}
