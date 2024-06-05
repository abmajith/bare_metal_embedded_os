.section ".text.boot"
.global _start

_start:
    // Determine which core is running this code
    mrs x1, mpidr_el1
    and x1, x1, #3       // Extract 2 LSB's represents core ID
    cbz x1, 2f           // If core ID is 0, jump to boot_core0

1:
    wfe                   // wait for event
    b 1b

2:
    ldr x1, =_start
    // change to exception level 1

    // enable AArch64 in EL1
    mrs x0, hcr_el2
    orr x0, x0, #(1 << 31)  // Set RW bit (bit 31) to 1 for AArch64 EL1
    msr hcr_el2, x0

    // Set up SPSR_EL2 for returning to EL1
    mov x0, #0x3C5        // EL1h, AArch64, interrupts disabled
    msr spsr_el2, x0
    adr x0, 3f            // address to return to after the exception return
    msr elr_el2, x0       // exception link register
    eret                  // exception return

3:
    mov sp, x1
    // clear bss
    ldr x5, =__bss_start
    ldr x6, =__bss_size
4:
    cbz x6, 5f
    str xzr, [x5], #8
    sub x6, x6, #1
    cbnz x6, 4b

5:
    bl kernel_main
    // If kernel_main returns, halt this core too
    b 1b

