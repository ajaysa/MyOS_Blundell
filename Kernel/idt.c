/* Defines an IDT entry */
struct idt_entry
{
    unsigned short base_low_bits;  // lower address bits of fault handler
    unsigned short selector;       // Our kernel segment goes here!
    unsigned char always0;         // This will ALWAYS be set to 0! 
    unsigned char flags;
    unsigned short base_high_bits; // higher address bits of fault handler
} __attribute__((packed));

// idt pointer structure : contains base address and limit
struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Declare an IDT of 256 entries.
   If any undefined IDT entry is hit, it normally
   will cause an "Unhandled Interrupt" exception. Any descriptor
   for which the 'presence' bit is cleared (0) will generate an
   "Unhandled Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in 'kernel_entry.asm', and is used to load our IDT */
extern void idt_load();

// This function sets an entry in the IDT.
void idt_set_gate(unsigned int num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_low_bits = (unsigned short)(base & 0xFFFF);
    idt[num].base_high_bits = (unsigned short)((base >> 16)  & 0xFFFF);
    idt[num].selector = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Points the processor's internal register to the new IDT */
    idt_load();
}
