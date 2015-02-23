#include <fnctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "kvmda.h"

void create_vm(kvm_t * kvm) {
    kvm->kvm_fd = open("/dev/kvm", O_RDRW);
    kvm->vm_fd = ioctl(kvm->kvm_fd, KVM_CREATE_VM, 0);
    ioctl(kvm->vm_fd, KVM_SET_TSS_ADDR, 0xffffffffffffd000);
    ioctl(kvm->vm_fd, KVM_CREATE_IRQCHIP, 0);
}

int add_phys_memory(kvm_t * kvm, int mem_size) {
    void * addr = mmap(NULL, mem_size * MB, PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    struct kvm_userspace_memory_region region = {
        .slot = 0,
        .flags = 0,
        .guest_phys_addr = 0x100000,
        .memory_size = mem_size * MB,
        .userspace_addr = (__u64)addr
    };

    return ioctl(kvm->vm_fd, KVM_SET_USER_MEMORY_REGION, &region);
}

int init_vcpu(kvm_t * kvm) {
    struct kvm_regs regs;

    kvm->vcpu_fd = ioctl(kvm->vm_fd, KVM_CREATE_VCPU, 0);
    ioctl(kvm->vcpu_fd, KVM_GET_REGS, &regs);
    regs.rflags = 0x02;
    regs.rip = 0x100f000;
    ioctl(kvm->vcpu_fd, KVM_SET_REGS, &regs);
}

int run_vcpu(kvm_t * kvm) {
    int res;
    int kvm_run_size;
    struct kvm_run * run_state;

    kvm_run_size = ioctl(kvm->kvm_fd, KVM_GET_CPU_MMAP_SIZE, 0);
    run_state = mmap(NULL, kvm_run_size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE, kvm->vcpu_fd, 0);

    while(1) {
        res = ioctl(kvm->vcpu_fd, KVM_RUN, 0);

        
    }
}

