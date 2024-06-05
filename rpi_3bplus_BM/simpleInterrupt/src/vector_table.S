.section .text
.align 11
.global vector_table

vector_table:
    // Synchronous EL1t
    b default_handler
    .align 7
    // IRQ EL1t
    b irq_handler
    .align 7
    // FIQ EL1t
    b fiq_handler
    .align 7
    // SError EL1t
    b default_handler
    .align 7
    // Synchronous EL1h
    b default_handler
    .align 7
    // IRQ EL1h
    b irq_handler
    .align 7
    // FIQ EL1h
    b fiq_handler
    .align 7
    // SError EL1h
    b default_handler
    .align 7
    // Synchronous EL0_64
    b default_handler
    .align 7
    // IRQ EL0_64
    b irq_handler
    .align 7
    // FIQ EL0_64
    b fiq_handler
    .align 7
    // SError EL0_64
    b default_handler
    .align 7
    // Synchronous EL0_32
    b default_handler
    .align 7
    // IRQ EL0_32
    b irq_handler
    .align 7
    // FIQ EL0_32
    b fiq_handler
    .align 7
    // SError EL0_32
    b default_handler
    .align 7

