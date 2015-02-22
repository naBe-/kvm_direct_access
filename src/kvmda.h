#include <stdio.h>
#include <stdlib.h>

typedef struct kvm {
    int kvm_fd, sys_fd, vcpu_fd;
    void * memory;
} kvm_t;
