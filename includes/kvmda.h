#define MB 1048576

typedef struct kvm {
    int kvm_fd, vm_fd, vcpu_fd;
    void * mem_addr;
} kvm_t;

void create_vm(kvm_t * kvm);
int add_phys_memory(kvm_t * kvm, int mem_size);
int init_vcpu(kvm_t * kvm);
int run_vcpu(kvm_t * kvm);
