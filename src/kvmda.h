#include <stdio.h>
#include <stdlib.h>

typedef struct kvm {
    int kvm_fd, vm_fd, vcpu_fd;
    void * mem_addr;
} kvm_t;

void create_vm(kvm_t * kvm);
int add_phys_memory(kvm_t * kvm, int mem_size);
