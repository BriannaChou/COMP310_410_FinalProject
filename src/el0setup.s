
/*
 * This code puts the CPU in execution level 0. You can paste it in to your
 * boot.s file right after stack pointer setup.
 *
 */
//maincore:
    // set stack before our code
//    ldr     x5, =_start
//    mov     sp, x5

/* Beginning of EL1 setup */

    // set stack before our code
    ldr     x1, =_start

    // set up EL1
    mrs     x0, CurrentEL
    and     x0, x0, #12 // clear reserved bits

    // running at EL3?
    cmp     x0, #12
    bne     5f
    // should never be executed, just for completeness
    mov     x2, #0x5b1
    msr     scr_el3, x2
    mov     x2, #0x3c9
    msr     spsr_el3, x2
    adr     x2, 5f
    msr     elr_el3, x2
    eret

    // running at EL2?
5:  cmp     x0, #4
    beq     5f
    msr     sp_el1, x1
    msr     sp_el0, x1
    // enable CNTP for EL1
    mrs     x0, cnthctl_el2
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr
    // enable AArch64 in EL1
    mov     x0, #(1 << 31)      // AArch64
    orr     x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
    msr     hcr_el2, x0
    mrs     x0, hcr_el2

    // set up exception handlers
    // Uncomment this stuff once you set up the vector table
//    ldr     x2, =_vectors
//    msr     vbar_el1, x2
//    msr     vbar_el2, x2

    // change execution level to EL1
    mov     x2, #0x3c4 // Change execution level to EL1
//    mov     x2, #0x3c0   // Change execution level to EL0
    msr     spsr_el2, x2
    adr     x2, 5f  /* Put the address of the instruction at label 5 (below) into register X2 */
    msr     elr_el2, x2
    eret

5:  mov     sp, x1

// CALL kernel_main after this...
