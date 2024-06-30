#!/bin/bash
#add args to specify the links to open
if [ $# -eq 0 ]; then
    echo "No arguments provided"
    exit 1
fi

# create a list of conditional statements to open the links
for var in "$@"; do
    if [ $var = "libbpf" ]; then
        links 'https://docs.kernel.org/bpf/libbpf/libbpf_overview.html'
    elif [ $var = "tracepoints" ]; then
        links 'https://kernel.org/doc/html/latest/trace/tracepoints.html'
    elif [ $var = "ringbuf" ]; then
        links 'https://nakryiko.com/posts/bpf-ringbuf/'
    elif [ $var = "bpfhub" ]; then
        links 'https://github.com/aquasecurity/btfhub'
    elif [ $var = "whirl" ]; then
        links 'https://qmonnet.github.io/whirl-offload/2020/04/11/tc-bpf-direct-action/'
    elif [ $var = "defcon20" ]; then
        links 'https://media.defcon.org/DEF%20CON%2029/DEF%20CON%2029%20presentations/Rex%20Guo%20Junyuan%20Zeng%20-%20Phantom%20Attack%20-%20%20Evading%20System%20Call%20Monitoring.pdf'
    elif [ $var = "falco" ]; then
        links 'https://github.com/falcosecurity'
    elif [ $var = "freetrial" ]; then
        links 'https://www.oreilly.com/library/view/security-observability-with/9781492096719/'
    elif [ $var = "bpftrace" ]; then
        links 'https://github.com/bpftrace/bpftrace/tree/master'
    elif [ $var = "windows" ]; then
        links 'https://github.com/microsoft/ebpf-for-windows#architectural-overview'
    elif [ $var = "ir" ]; then
        links 'https://lwn.net/Articles/755752/'
    else
        echo "No link found for $var"
    fi
done

#write a for loop to open the links

#links 'https://docs.kernel.org/bpf/libbpf/libbpf_overview.html'

#ringbuf
#links https://nakryiko.com/posts/bpf-ringbuf/

#btfhub
#https://github.com/aquasecurity/btfhub

#tc whirl offload
#https://qmonnet.github.io/whirl-offload/2020/04/11/tc-bpf-direct-action/

#ir 
#https://lwn.net/Articles/755752/

#falco
#https://github.com/falcosecurity
#https://media.defcon.org/DEF%20CON%2029/DEF%20CON%2029%20presentations/Rex%20Guo%20Junyuan%20Zeng%20-%20Phantom%20Attack%20-%20%20Evading%20System%20Call%20Monitoring.pdf

#security observability with ebpf 10 day free trial
#https://www.oreilly.com/library/view/security-observability-with/9781492096719/

#bpftrace
#https://github.com/bpftrace/bpftrace/tree/master

#ebpf for windows
#https://microsoft.github.io/ebpf-for-windows/
#https://github.com/microsoft/ebpf-for-windows#architectural-overview


#tracepoints
#links https://kernel.org/doc/html/latest/trace/tracepoints.html

