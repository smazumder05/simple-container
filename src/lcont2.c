#define _GNU_SOURCE
#include<sched.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/mount.h>
#include<sys/mount.h>
#include<sys/stat.h>
#include<sys/syscall.h>
#include<unistd.h>



static char child_stack[1024 * 1024];


int  print_child(void *args) {
    printf("PID as seen from the child: %d\n", getpid());
    /** Unmount procfs and pivot roor instead*/
    unshare(CLONE_NEWNS);
    umount2("/proc", MNT_DETACH);
    /** Unmount procfs and pivot roor instead*/
    mount("./rootfs","./rootfs","bind",MS_BIND | MS_REC, "");
    mkdir("./rootfs/oldrootfs",0755);
    syscall(SYS_pivot_root,"./rootfs","./rootfs/oldrootfs");
    chdir("/");
    umount2("oldrootfs", MNT_DETACH);
    rmdir("/oldrootfs");
    /** Remount procfs */
    mount("proc", "/proc","proc",0,NULL);
    /** Now run the process.*/
    char **argv = (char **)args;
    //execvp(argv[0], argv);
    system("bash");
    return 0;
}

int main(int argc,char *argv[]) {
    int namespaces = CLONE_NEWUTS|CLONE_NEWPID|CLONE_NEWIPC|CLONE_NEWNS|CLONE_NEWNET;
    pid_t p = clone(print_child, child_stack + sizeof(child_stack), SIGCHLD|namespaces, argv + 1);
    if ( p == -1 ) {
        perror("Clone: clould not clone the process.");
        exit(1);
    }
    printf("Child pid is: %d\n",p);
    waitpid(p,NULL,0);
    return 0;
}
