#ifndef __EXEC_H
#define __EXEC_H

struct event 
{
    int pid;
    int ppid;
    unsigned exit_code;
    unsigned long long duration_ns;
    char comm[16];
    char filename[127];
    bool exit_event;
};

#endif // __EXEC_H
