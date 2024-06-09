#include <stdio.h>
#include <unistd.h>
//https://github.com/lizrice/learning-ebpf

int
bpf_hello_world(void)
{
    printf("Hello, BPF World!\n");
    return 0;
}

int
run_cmd(const char* cmd, char* const args[])
{
    return execvp(cmd, args);
}

int
hello(char* msg)
{
    printf("Hello, %s!\n", msg);
    return 0;
}

int
main(int argc, char* argv[])
{
    int r = 0;
    hello("world");
    bpf_hello_world();
    char* const args[] = {"strace","-c","cal", NULL};
    if (r = run_cmd(args[0], args) < 0) 
    {
        perror("execvp");
        return r;
    }
    //char* const as[] = {"ps", "-al", NULL};
    //execve("/bin/ls", as, NULL);

    return 0;
}
