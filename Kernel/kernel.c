#include <kernel.h>

#ifndef _KHEAP
#include <memory.h>
#endif

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
	print("\nHeap Memory SPEC :: \n");
	kheap_SPEC();
	print("\n\n");

	print(_CMD);

	// scanf testing
/*	print("Enter a Number :: \n");
	scan('d', &num);
*/
	// heap memory test
/*	char *test, *temp;
	int i;
	test = (char *)kmalloc(10);

	if ( !test )
		print("memory allocation failed\n");
	else
	{
		temp = test;
		print("memory allocated at :: ");
		print_hex(temp);

		for(i=0; i<10; i++)
		{
			*temp='A';
			temp++;
		}

		temp = test;
		for(i=0; i<10; i++)
		{
			printchar(*temp);
			temp++;
		}

		temp = NULL;
		kfree(test);
	}
*/
	// test : running of exception handlers
	//print(20/0);

	// check memory map
/*	char *addr = (char *)0x5000000;
	do
	{
		printchar(*addr);
		*addr = '9';
		addr++;
	}while(addr < 0x5000100);

	addr = (char *)0x5000000;
	do
	{
		printchar(*addr);
		addr++;
	}while(addr < 0x5000100); */
}
