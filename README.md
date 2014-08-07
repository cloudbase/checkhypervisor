checkhypervisor
===============

Utility to detect if the OS is running as a guest on a known hypervisor

## Linux build instructions

The following build command will generate a statically linked 32bit executable: 

    ./build.sh

## Windows build instructions

The following build command will generate a statically linked executable: 

    ./build.cmd

Note: use the x86 Visual Studio toolchain to obtain a 32bit executable.

## Running

    ./checkhypervisor
    
Returns the ID provided by the hypervisor, if present. For example:

* Hyper-V:
    **Microsoft Hv**
* KVM:
    **KVMKVMKVM**
* VMWare:
    **VMwareVMware**

## Exit code

The exit code can be either 0 or 1:

* 0: hypervisor found
* 1: hypervisor not found
