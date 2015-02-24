#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "kvmda.h"

/**
 * @file kvmda.c
 * @brief KVM direct access uses ioctl calls to the kernel KVM interface
 * 
 * This program has been produced following Gabriel Lascar's lecture
 * @see [KVM without Qemu](https://lse.epita.fr/lse-summer-week-2013/slides/lse-summer-week-2013-16-Gabriel%20Laskar-KVM%20Without%20QEMU.pdf)
 *
 * @author Pavel Bliznakov
 * @date 24/02/2015
 * 
 */

/**
 * @brief Initiate virtual machine via the kvm virtual device
 *
 * @param kvm pointer to struct, containing information on the virtual machine
 *
 */
void create_vm(kvm_t * kvm) {
    kvm->kvm_fd = open("/dev/kvm", O_RDWR);
    kvm->vm_fd = ioctl(kvm->kvm_fd, KVM_CREATE_VM, 0);

    /* This is needed on Intel hardware in order to define the physical address of
     * a three-page region in the first 4GB of the guest memory space.
     * Needed because of virtualization implementation on Intel.
     */
    ioctl(kvm->vm_fd, KVM_SET_TSS_ADDR, 0xffffffffffffd000);

    /* Create interrupt controller model in the kernel. */
    ioctl(kvm->vm_fd, KVM_CREATE_IRQCHIP, 0);
}

/**
 * @brief Set memory to the virtual machine
 * 
 * Takes two parameters: @p kvm, containing general parameters of the virtual machine and @p mem_size,
 * the memory in MB to add to the virtual machine.
 *
 * @param kvm pointer to struct, containing information on the virtual machine
 * @param mem_size virtual machine memory size in MB
 * @return -1 if memory allocation fails, 0 otherwise.
 *
 */
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
    
    kvm->mem_addr = addr; /* Store memory address in virtual machine data. */

    /* Allocate the memory. */
    return ioctl(kvm->vm_fd, KVM_SET_USER_MEMORY_REGION, &region);
}

/**
 * @brief Initialize 64bit virtual CPU. Set registers.
 *
 * @param kvm pointer to struct, containing information on the virtual machine
 * @return 0 if the CREATE_VCPU ioctl call succeeds, -1 if not.
 *
 */
int init_vcpu(kvm_t * kvm) {
    struct kvm_regs regs;

    kvm->vcpu_fd = ioctl(kvm->vm_fd, KVM_CREATE_VCPU, 0);
    if(kvm->vcpu_fd < 0)
        return -1;
    ioctl(kvm->vcpu_fd, KVM_GET_REGS, &regs);
    regs.rflags = 0x02; /* Initialise 64bit status register. */
    regs.rip = 0x100f000; /* Set the instruction pointer (program counter) */
    ioctl(kvm->vcpu_fd, KVM_SET_REGS, &regs);
    return 0;
}

/**
 * @brief Run the virtual CPU
 *
 * @param kvm pointer to struct, containing information on the virtual machine
 * @return Exit code of the VCPU.
 *
 */
int run_vcpu(kvm_t * kvm) {
    int res;
    int kvm_run_size;
    struct kvm_run * run_state;

    kvm_run_size = ioctl(kvm->kvm_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
    run_state = mmap(NULL, kvm_run_size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE, kvm->vcpu_fd, 0);

    /* Main loop for the VCPU */
    while(1) {
        res = ioctl(kvm->vcpu_fd, KVM_RUN, 0);
        fprintf(stderr, "res: %d\n", res); 
    }
    return run_state->exit_reason;
}

