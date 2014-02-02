#include <kernel.h>

// These are own ISRs that point to our special IRQ handler
// instead of the regular 'fault_handler' function
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// This array is actually an array of function pointers. It will be 
// used to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/* remapping IRQ0 to IRQ15 to ISR32 to ISR47
   Normally :
    IRQ 0..7 - INT 0x8..0xF     (8-15 in decimal)
    IRQ 8..15 - INT 0x70..0x77  (112-119 in decimal) 
   When you enter protected mode (or even before hand, if you're not using GRUB) the first command
   you will need to give the two PICs is the initialise command (code 0x11). This command makes 
   the PIC wait for 3 extra "initialisation words" on the data port. These bytes give the PIC:
    - Its vector offset. (ICW2)
    - Tell it how it is wired to master/slaves. (ICW3)
    - Gives additional information about the environment. (ICW4) 
   
   Master - command: 0x20, data: 0x21
   Slave - command: 0xA0, data: 0xA1
*/
void irq_remap(void)
{
    port_byte_put(0x20, 0x11); /* write ICW1 to PICM, we are gonna write commands to PICM */
    port_byte_put(0xA0, 0x11); /* write ICW1 to PICS, we are gonna write commands to PICS */
    
    port_byte_put(0x21, 0x20); /* remap PICM to 0x20 (32 decimal) */
    port_byte_put(0xA1, 0x28); /* remap PICS to 0x28 (40 decimal) */
    
    port_byte_put(0x21, 0x04); /* IRQ2 -> connection to slave */ 
    port_byte_put(0xA1, 0x02);
    
    port_byte_put(0x21, 0x01); /* write ICW4 to PICM, we are gonna write commands to PICM */
    port_byte_put(0xA1, 0x01); /* write ICW4 to PICS, we are gonna write commands to PICS */
    
    port_byte_put(0x21, 0x0); /* enable all IRQs on PICM */
    port_byte_put(0xA1, 0x0); /* enable all IRQs on PICS */
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_install()
{
    irq_remap();

    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void irq_handler(struct regs *r)
{
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        port_byte_put(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    port_byte_put(0x20, 0x20);
}
