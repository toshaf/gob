#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

int gob_trace(char *const path) {

    int pid = fork();
    if (pid < 0)
        return pid;

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);

        char *argv[] = { path, NULL };
        char *envp[] = { NULL };

        execve(path, argv, envp);
    }

    int status = 0;
    while (1) {
        wait(&status);

        if (WIFEXITED(status)) {
            break;
        }

        if (WIFSTOPPED(status)) {
           struct user_regs_struct regs;
           ptrace(PTRACE_GETREGS, pid, NULL, &regs);

           printf("RIP: %llu\n", regs.rip);

           ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
        }
    }

    return 0;
}
