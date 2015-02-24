KVM direct access
=================

Demonstrates direct access to the Linux kernel KVM module by initiating and running a virtual CPU.

## Compile the project
```
make
```

## Run the program
```
bin/kvm_da [-m <memory in MB>]
```

## Generate documentation

Doxygen is required to generate documentation. You simply need to run:
```
make doc
```
Once the generation is completed, you can find the documentation in the ```doc/``` directory, available in HTML and LaTeX format.

## Clean up
```
make clean
```
