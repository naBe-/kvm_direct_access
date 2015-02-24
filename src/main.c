#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kvmda.h"

void usage(char * prog) {
    printf("Usage: %s -b <binary> [-m <memory size>]\n", prog);
}

int main(int argc, char * argv[]) {
    char key;
    kvm_t kvm;
    int mem_size;

    mem_size = 256;

    while((key = getopt(argc, argv, "m:b:")) != -1)
        switch(key) {
            case 'm':
                mem_size = atoi(optarg);
                break;
            case '?':
                if(optopt == 'm' || optopt == 'b')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else
                    fprintf(stderr, "Unknown option: %c\n", optopt);
                exit(EXIT_FAILURE);
            default:
                usage(argv[0]);
                exit(EXIT_SUCCESS);
        }

    create_vm(&kvm);
    if(add_phys_memory(&kvm, mem_size) < 0) {
        fprintf(stderr, "Cannot set memory.\n");
        exit(EXIT_FAILURE);
    }
    if(init_vcpu(&kvm) < 0) {
        fprintf(stderr, "Cannot initialize VCPU.\n");
        exit(EXIT_FAILURE);
    }
    return run_vcpu(&kvm);
}
