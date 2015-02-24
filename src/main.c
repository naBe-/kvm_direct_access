#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kvmda.h"

/**
 * @file main.c
 * @brief Main function wrapping direct access to the KVM kernel module
 * 
 * This program has been produced following Gabriel Lascar's lecture
 * @see [KVM without Qemu](https://lse.epita.fr/lse-summer-week-2013/slides/lse-summer-week-2013-16-Gabriel%20Laskar-KVM%20Without%20QEMU.pdf)
 * @see [KVM module API](https://www.kernel.org/doc/Documentation/virtual/kvm/api.txt)
 *
 * @author Pavel Bliznakov
 * @date 24/02/2015
 * 
 */

/**
 * @brief Indicate how to use the command
 *
 * Takes one parameter @p prog, which is the executable name. Prints out usage message.
 *
 * @param prog executable file name
 *
 */
void usage(char * prog) {
    printf("Usage: %s [-m <memory size>]\n", prog);
}

/**
 * @brief Program entry point
 *
 */
int main(int argc, char * argv[]) {
    char key; /* Used to get keys from the command line. */
    kvm_t kvm; /* Strore information on the current KVM instance. */
    int mem_size; /* Virtual machine's allocated memory size. */

    mem_size = 256; /* Initiate memory size to 256MB by default. */

    /* Parse command line options */
    while((key = getopt(argc, argv, "m:")) != -1)
        switch(key) {
            case 'm':
                /* In case the -m option is specified,
                 * get its value and set it as virtual
                 * machine's memory size
                 */
                mem_size = atoi(optarg);
                break;
            case '?':
                if(optopt == 'm')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else
                    fprintf(stderr, "Unknown option: %c\n", optopt);
                exit(EXIT_FAILURE);
            default:
                usage(argv[0]);
                exit(EXIT_SUCCESS);
        }

    /* Open /dev/kvm and create vm instance. */
    create_vm(&kvm);

    /* Set virtual machine's memory */
    if(add_phys_memory(&kvm, mem_size) < 0) {
        fprintf(stderr, "Cannot set memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Set one virtual CPU for the virtual machine */
    if(init_vcpu(&kvm) < 0) {
        fprintf(stderr, "Cannot initialize VCPU.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Press Ctrl+C to interrupt...\n");
    /* Execute loaded code */
    return run_vcpu(&kvm);
}
