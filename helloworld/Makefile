
APP=main

.PHONY: $(APP)
$(APP): skel
	clang main.c -lbpf -lelf -o $(APP)

.PHONY: vmlinux 
vmlinux:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
	
.PHONY: bpf
bpf: vmlinux
	clang -g -O3 -target bpf -D__TARGET_ARCH_x86_64 -c main.bpf.c -o main.bpf.o

.PHONY: skel
skel: bpf
	bpftool gen skeleton main.bpf.o name main > main.skel.h 
	rm main.bpf.o

.PHONY: run
run: $(APP)
	sudo ./$(APP)

.PHONY: clean
clean:
	rm -f $(APP) main.bpf.o main.skel.h vmlinux.h

.PHONY: gdb
gdb: bpf
	sudo gdb --args bpftool gen skeleton main.bpf.o name main
