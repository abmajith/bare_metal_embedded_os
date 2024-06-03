## Hello Interrupts


In this folder, we continue the hello peripherals program and extend it with interrupts.

To understand interrupts, we have to go through with exceptions first, as it requires elaborate
discussion, I refer to the original <a href="https://developer.arm.com/documentation/den0024/a/Introduction" class="custom-link">ARM documentation page</a>
which is more practical than the ARMv8 reference manual (really big!).

As we know, *Ras pi 3b plus* has Broadcom 2837 a cortex A57 A profile ARM processor, based on  ARMv8 architecture.


<img src="https://github.com/abmajith/bare_metal_embedded_os/tree/main/rpi_3bplus_BM/helloInterruptsSimplePartOne/exceptionsLevel.png" alt="J" width="400"/>

As you see, the armv8-based architecture has 4 levels of exceptions.


- EL0: For normal user-level applications (like user-level applications in Linux) (least privileged)
- EL1: Operating System (Linux kernel, root user)
- EL2: a single exception level called Hypervisor that provides virtualization services if it is enabled.
- Low-level firmware, including the secure monitor.


It is ideal to develop our *OS* tutorial in EL1 level, as you see, we are not developing software that accommodates multiple *OS* kernel services.
Also, we are not monitoring the service, rather we are in the direction of developing a bare metal *OS*.
