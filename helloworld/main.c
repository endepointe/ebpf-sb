#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <assert.h>
//#include <linux/bpf.h>
#include <sys/resource.h>

#include "main.skel.h"

static void bump_memlock_rlimit(void)
{
    struct rlimit rlim = {
        .rlim_cur = RLIM_INFINITY,
        .rlim_max = RLIM_INFINITY,
    };
    if (setrlimit(RLIMIT_MEMLOCK, &rlim))
    {
        fprintf(stderr, "Failed to increase rlimit: %m\n");
        exit(1);
    }
}

int
main(void)
{
    bump_memlock_rlimit();
    struct main *skel = main__open();
    main__load(skel);
    main__attach(skel);

    for (;;) {
    }
    return 0;
}
