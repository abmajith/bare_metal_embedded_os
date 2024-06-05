## Hello Interrupts


In this folder, we continue the hello peripherals program and extend it with interrupts.

To understand interrupts, we have to go through with exceptions first, as it requires elaborate
discussion, I refer to the original <a href="https://developer.arm.com/documentation/den0024/a/Introduction" class="custom-link">ARM documentation page</a>
which is more practical than the ARMv8 reference manual (really big!).

As we know, *Ras pi 3b plus* has Broadcom 2837 a cortex A57 A profile ARM processor, based on ARMv8 architecture.



## Execution or Exception Levels
![](exceptionsLevel.png)

As you see, the armv8-based architecture has 4 levels of exceptions.


- EL0: For normal user-level applications (like user-level applications in Linux) (least privileged)
- EL1: Operating System (Linux kernel, root user)
- EL2: a single exception level called Hypervisor that provides virtualization services if it is enabled.
- EL3: Low-level firmware, including the secure monitor.


It is ideal to develop our *OS* tutorial in EL1 level, as you see, we are in the direction of developing a bare metal *OS*.


Compared to earlier hello UART0, UART1, and peripherals folder, here we have to change our start.S assembly file to switch into
level *EL1* from *EL2*. To switch from *EL2* to *EL1*, we will get to know a few register types.

To find our current execution level we use the following C function

```C
unsigned int get_current_el()
{
  unsigned int el;
  asm volatile ("mrs %0, CurrentEL" : "=r"(el));
	// mrs is a special load function for the system registers value, CurrentEL is a system register
  return (el >> 2) & 3; // Two LSB's are not important here i.e el[3:2] gives us the execution privilege level
}
```


## General Purpose and System Registers

There are 31 general-purpose registers named *X0, X1,...X30* (62-bit width), the same registers can be used as 32-bit width,
if we refer to them as *W0, W1,...W30*. These registers are common for all four exception levels (or execution levels).

There are also several special registers.

Zero register *XZR/WZR*  (for 62 and 32 bits mode respectively), and program counter *PC* are common for all four levels.

There are Stack pointer registers *SP_ELn*, where n= 0,1,2 or 3 for each exception level (*WSP_ELn* for 32 bit mode). 
In the program when we refer *SP*, it refers to the current exception level stack pointer register by default.
This reference can be modified, but the level *EL0* can only refer to *SP_EL0*. I.e when a processor at exception level
other than *EL0* can refer to default stack pointer *SP_EL0* or *SP_ELn*.

There is also a Exception link register *ELR_ELn* (holds exception return address) and 
Saved Processor Status Registers *SPSR_ELn* for n= 1, 2 or 3.

I.e *ELR_ELn* store the return address.
More processor specific information stored in *Saved program Status Register*
exception level it should go back, that information stored in the *SPSR* register. 
-	*SPSR_ELn*[3:0] holds exception level and accessing of *SP_EL0/SP_ELn*
-	*SPSR_ELn*[4] 0 indicates aarch64, otherwise it is *aarch32*.
-	*SPSR_ELn*[9:6] = *DAIF*, (Debug, Accesses System Error, IRQ, FIQ mask bit)
-	*SPSR_ELn*[31:28] = *NZCV* (Negative, Zero, Carry, Overflow flags)

For an example in order to set the exception level to EL1 in EL2 and access *SP_EL1*, and set the aarch mode in 64-bits and masking 
*DAIF*, we could set like this

```asm
	 mov x0, #0x3C5        // EL1h, AArch64, interrupts disabled
	 msr spsr_el2, x0
```

There are number of System registers notably our *SPSR_ELn*, *ELR*, and many others.

Some important registers that will be used currently
*HCR_EL2* Hypervisor Configuration  Register, if our required exception and interrupts should be handled at Exception level *2* 
(I.e Hypervisor) for the low level exception application and programs, this register should be set with appropriate values. 
The one thing we should be set for our case is the lower level exception should run at *aarch64* mode for that
we set the 31st bit of *hcr_el2*. There are many more system register are neeeded for the kernel development, will see them in future.


## Change of Exception Levels

-	The current instruction address should be saved in the *ELR_ELn* (Exception link register for current execution level *ELn*)
-	The current processor state (like exeception level, aarch mode, endieness etc) in *SPSR_ELn* (Saved program status register at *ELn* level)
-	Now call the *Exception Handler*, a subroutine program to analyse and perform appropriate actions.
-	Return (*eret* instruction) from the Exception handler and restore the program status by loading the values *ELR_ELn* to the stack pointer, and restoring the 
	processor state by accessing *SPSR_ELn*.


To change from exception level 2 to exception level 1, and manage everything within the *OS* level (i.e disabling hypervisor)

```asm
	ldr x0 #(1 << 31)
	msr hcr_el2, x0
	// Set up SPSR_EL2 for returning to EL1
	mov x0, #0x3C5        // EL1h (to set up the usage of SP_EL1), AArch64, interrupts disabled
	msr spsr_el2, x0
	adr x0, startel1
	msr elr_el2, x0
	eret // change to new exception level
startel1:
	// various initialization and kernel_main function
```

## AArch64 exception-Interrupt table


When an exception interrupt occurs, the processor must execute handler code which corresponds to the exception. 
The location in memory where the handler is stored is called the exception vector (or interrupt vector). 
In the ARM architecture, exception vectors are stored in a table, called the exception vector table. 
Each Exception level has its own vector table, that is, there is one for each of *EL3*, *EL2* and *EL1*. 

The table contains instructions to be executed, rather than a set of addresses. 
Vectors for individual exceptions are located at fixed offsets from the beginning of the table. 
The virtual address of each table base is set by the Vector Based Address Registers *VBAR_EL3*, *VBAR_EL2* and *VBAR_EL1*.


In order write the system based interrupt for counter or delay instead of polling type we have to use these *VBAR_EL1*. 


## Vector Table Offsets
address with offset   |     Exception Interrupts  |         Description          
----------------------|---------------------------|---------------------------------
*VBAR_ELn + 0x000*    |    Synchronous            |         *SP_EL0*
*+ 0x080*             |     IRQ                   |  The exception is taken from *EL1*  
*+ 0x100*             |     FIQ                   |   but *EL1, EL0* shares the stack pointer *SP_EL0*
*+ 0x180*             |     SError                |      
----------------------|---------------------------|---------------------------------
----------------------|---------------------------|---------------------------------
*+ 0x200*             |    Synchronous            |            *SP_EL1*
*+ 0x280*             |     IRQ                   |   The exception is take from *EL1*
*+ 0x300*             |     FIQ                   |			but *EL1, EL2* using their own stack pointer registers
*+ 0x380*             |     SError                |				*SP_EL0, SP_EL1*
----------------------|---------------------------|----------------------------------
----------------------|---------------------------|----------------------------------
*+ 0x400*             |    Synchronous            |   *SP_EL0* using AARCH64 mode
*+ 0x480*             |     IRQ                   |			Exception is taken from *EL0*
*+ 0x500*             |     FIQ                   |			that executes in 64-bit mode
*+ 0x580*             |     SError                |
----------------------|---------------------------|----------------------------------
----------------------|---------------------------|----------------------------------
*+ 0x600*             |    Synchronous            |   *SP_EL0* using AARCH32 mode
*+ 0x680*             |     IRQ                   |			Exception is taken from *EL0*
*+ 0x700*             |     FIQ                   |     that executes in 32-bit mode
*+ 0x780*             |     SError                |
----------------------|---------------------------|----------------------------------




-	The type of exception (SError, FIQ, IRQ or Synchronous).

-	If the exception is being taken at the same Exception level, the Stack Pointer to be used can vary, its either *SP_EL0* or *SP_EL1*, depends on how we set the 
saved processor status register.

- If the exception is being taken at a lower Exception level i.e , the execution state of the next lower level (AArch64 or AArch32)

