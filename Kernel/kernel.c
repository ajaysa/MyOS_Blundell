void some_function()
{
}

void main()
{
	char *video_memory = (char *)0xb8000;
	*video_memory = 'X';
	some_function();

	init_video();

	print("Hello World\n");
}
