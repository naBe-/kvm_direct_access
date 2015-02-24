#include <stdio.h>
#include <stdlib.h>

#include "kvmda.h"

int main(int argc, char * argv[]) {
    kvm_t kvm;
    create_vm(&kvm);
    if(add_phys_memory(&kvm, 256) < 0) {
        fprintf(stderr, "Cannot set memory.");
        return EXIT_FAILURE;
    }
    if(init_vcpu(&kvm) < 0) {
        fprintf(stderr, "Cannot initialize VCPU.");
        return EXIT_FAILURE;
    }
    return run_vcpu(&kvm);
}
