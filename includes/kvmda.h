/**
 * KVM direct access interfaces
 *
 * @author Pavel Bliznakov
 * @date 24/02/2015
 *
 */

#ifndef KVMDA_H
#define KVMDA_H

#define MB 1048576

/**
 * @brief Defines type to store file descriptors and memory address relative to the KVM instance.
 */
typedef struct kvm {
    int kvm_fd; /**< Contains the file descriptor to /dev/kvm */
    int vm_fd; /**< Contains the file descriptor the voirtual machine */
    int vcpu_fd; /**< Contains the file descriptor to the VCPU */
    void * mem_addr; /**< Pointer to the virtual machine memory region */
} kvm_t;

void create_vm(kvm_t * kvm);
int add_phys_memory(kvm_t * kvm, int mem_size);
int init_vcpu(kvm_t * kvm);
int run_vcpu(kvm_t * kvm);

#endif /* KVMDA_H */
