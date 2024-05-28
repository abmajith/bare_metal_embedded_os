// AARCH64 mode

.section ".text.boot"
.global _start


_start:
  // Determine which core is running this code
  mrs	x1, mpidr_el1
  and	x1, x1, #3       // Extract 2 LSB's represents core ID

  cbz	x1, 2f           // If core ID is 0, jump to boot_core0
1: 
  wfe                  // If not core 0, wait for event (put core to sleep)
  b 1b                 // Loop indefinetely


  // set stack before our code
2:
	ldr x5, =_start
  mov sp, x5

  // clear bss
  ldr x5, =__bss_start
  ldr w6, =__bss_size
3:
	cbz w6, 4f
	str xzr, [x5], #8
	sub w6, w6, #1
	cbnz w6, 3b
	
  // Call the main C function
4:
	bl kernel_main
  // If kernel_main returns, halt this core too
  b 1b

