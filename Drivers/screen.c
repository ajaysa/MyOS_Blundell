#include "screen.h"

unsigned short *textpointer;
int cur_x = 0;
int cur_y = 0;

void move_cursor()
{
	unsigned short temp;
	temp = cur_y*MAX_COLUMNS + cur_x;
	
	// This sends a command to indicies 14 and 15 in the
	// CRT Control Register of the VGA controller. These
	// are the high and low bytes of the index that show
	// where the hardware cursor is to be 'blinking'.
	port_byte_put(SCREEN_CTRL_REG, 14);
	port_byte_put(SCREEN_DATA_REG, temp >> 8);
	port_byte_put(SCREEN_CTRL_REG, 15);
	port_byte_put(SCREEN_DATA_REG, temp);
}

void cls()
{
	unsigned short blank;
	unsigned short attribute_byte;
	int i;

	attribute_byte = WHITE_ON_BLACK << 8;
	blank = attribute_byte | 0x20;

	for(i=0; i<MAX_ROWS; i++)
		memsetw(textpointer+ i*MAX_COLUMNS, blank, MAX_COLUMNS);

	cur_x = 0;
	cur_y = 0;
	move_cursor();
}

void scroll()
{
	unsigned short temp, blank, attribute_byte;

	attribute_byte = WHITE_ON_BLACK << 8;	
	blank = attribute_byte | 0x20;

	if ( cur_y >= MAX_ROWS )
	{
		temp = cur_y - MAX_ROWS + 1;
		memcpy(textpointer, textpointer + temp*MAX_COLUMNS, (MAX_ROWS - temp) * MAX_COLUMNS * 2);

		memsetw(textpointer + (MAX_ROWS - temp)*MAX_COLUMNS, blank, MAX_COLUMNS);
		cur_y = MAX_ROWS - 1;
	}
}

void printchar(const char ch)
{
	unsigned short *where;
	unsigned short attribute_byte;

	attribute_byte = WHITE_ON_BLACK << 8;

	if ( ch == 0x08 )  // handle backspace
	{
		if ( cur_x > 0 )
			cur_x--;
	}

	if ( ch == 0x09 ) // handle tab
		cur_x = (cur_x + 8) & ~(8-1);

	if ( ch == '\r' )
		cur_x = 0;

	if ( ch == '\n' )
	{
		cur_y++;
		cur_x = 0;
	}

	// Any character greater than and including a space, is a
	// printable character. 
	if ( ch >= ' ' )
	{
		where = textpointer + cur_y*MAX_COLUMNS + cur_x;
		*where = attribute_byte | ch;
		cur_x++;
	}

	if ( cur_x >= MAX_COLUMNS )
	{
		cur_x = 0;
		cur_y++;
	}

	/* Scroll the screen if needed, and finally move the cursor */
	scroll();
	move_cursor();
}

void print(const char *str)
{
	int i;
	for(i=0; str[i]!='\0'; i++)
		printchar(str[i]);
}

void init_video()
{
	textpointer = (unsigned short *)VIDEO_MEMORY;
	cls();
}
