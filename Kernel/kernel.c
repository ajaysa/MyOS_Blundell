#include <kernel.h>

void some_function()
{
}

void main()
{
	char *video_memory = (char *)0xb8000;
	*video_memory = 'X';
	some_function();

	init_video();

	idt_install();
	isr_install();
	irq_install();

	timer_install();
	keyboard_install();

	__asm__ __volatile__ ("sti");

	print("Hello World\n");

	print("\n\tFrom Ajay Saini!\n");

	print(_CMD);

	// test : running of exception handlers
	//print(20/0);
}
