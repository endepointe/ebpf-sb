#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

//SEC("/sys/kernel/tracing/events/syscalls/sys_enter_execve")
int
handle_main(void *ctx)
{
    bpf_printk("main hello bpf world!!! ep\n");
    return 0;
}
