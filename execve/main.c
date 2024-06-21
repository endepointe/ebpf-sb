#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <bpf/libbpf.h>
#include <sys/resource.h>
#include "main.h"
#include "main.skel.h"

static volatile bool exiting = false;

static void sig_handler(int sig) 
{
    exiting = true;
}

static struct env {
	bool verbose;
	long min_duration_ms;
} env;

static void bump_memlock_rlimit(void)
{
	struct rlimit rlim_new = {
		.rlim_cur	= RLIM_INFINITY,
		.rlim_max	= RLIM_INFINITY,
	};

	if (setrlimit(RLIMIT_MEMLOCK, &rlim_new)) {
		fprintf(stderr, "Failed to increase RLIMIT_MEMLOCK limit!\n");
		exit(1);
	}
}

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
	if (level == LIBBPF_DEBUG && !env.verbose)
		return 0;
	return vfprintf(stderr, format, args);
}

static int
handle_event(void *ctx, void *data, size_t data_sz)
{
    const struct event *e = data;
    struct tm *tm;
    char ts[32];
    time_t t;

    time(&t);
    tm = localtime(&t);
    strftime(ts, sizeof(ts), "%H:%M:%S", tm);
    if (e->exit_event)
    {
        printf("%s: pid %d exited\n", ts, e->pid);
    }
    else
    {
        printf("%s: pid %d started\n", ts, e->pid);
    }
    return 0;
}

int
main(void)
{
    int err;
    struct ring_buffer *rb = NULL;

    //libbpf_set_print(libbpf_print_fn);

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    struct main *skel = main__open();

    if (!skel)
    {
        fprintf(stderr, "Failed to open and load BPF skeleton\n");
        return 1;
    }

    err = main__load(skel);
    if (err)
    {
        fprintf(stderr, "Failed to load and verify BPF skeleton\n");
        goto cleanup;
    }

    err = main__attach(skel);

    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton\n");
        goto cleanup;
    }

    rb = ring_buffer__new(bpf_map__fd(skel->maps.rb), handle_event, NULL, NULL);

    if (!rb) 
    {
        err = -1;
        fprintf(stderr, "fialed to create ring buffer\n");
        goto cleanup;
    }

	printf("%-8s %-5s %-16s %-7s %-7s %s\n", "TIME", "EVENT", "COMM", "PID", "PPID",
	       "FILENAME/EXIT CODE");

    while (!exiting)
    {
        err = ring_buffer__poll(rb, 100); // 100ms
        printf("ring_buffer__poll: %d\n", err);
        if (err == -EINTR)
        {
            err = 0;
            break;
        }
        if (err < 0)
        {
            printf("Error polling perf buffer: %d\n", err);
            break;
        }
    }
cleanup:
    ring_buffer__free(rb);
    main__destroy(skel);
    return err < 0 ? -err : 0;

}
