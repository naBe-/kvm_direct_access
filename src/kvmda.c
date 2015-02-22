#include <bool.h>
#include <fnctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include "kvmda.h"

int create_vm(kvm_t * kvm) {
    kvm->kvm_fd = open("/dev/kvm", O_RDRW);
    kvm->vm_fd = ioctl(kvm->kvm_fd, KVM_CREATE_VM, 0);
    
}
