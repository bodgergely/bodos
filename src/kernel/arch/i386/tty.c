#include <kernel/header.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"
#include "cursor.h"


size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = vga_buffer_address;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_scroll()
{

	for(size_t y = 1; y < VGA_HEIGHT;y++)
	{
		for(size_t v = 0; v < VGA_WIDTH; v++ )
		{
			size_t index = y*VGA_WIDTH+v;
			terminal_buffer[index-VGA_WIDTH] = terminal_buffer[index];
		}
	}
	for(size_t i=0;i<VGA_WIDTH;i++)
	{
		terminal_buffer[VGA_WIDTH*(VGA_HEIGHT-1) + i] = ' ';
	}
}

static void terminal_putchar(char c) {

	if(terminal_row == VGA_HEIGHT)
	{
		terminal_scroll();
		terminal_row = VGA_HEIGHT-1;
		terminal_column = 0;
	}


	if(c == '\n')
	{
		terminal_column = 0;
		if(++terminal_row == VGA_HEIGHT)
		{
			terminal_scroll();
			--terminal_row;
		}
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		++terminal_row;
	}
}

int terminal_putc(char c)
{
	terminal_putchar(c);
	move_cursor(VGA_WIDTH*terminal_row + terminal_column);
}

int terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
	move_cursor(VGA_WIDTH*terminal_row + terminal_column);

	return size;
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
