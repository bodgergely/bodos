#include "cursor.h"

 /* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15


inline static void send_high(uint16_t where)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, (where >> 8) & 0x00ff);
}

inline static void send_low(uint16_t where)
{
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, where & 0x00ff);
}

void move_cursor(uint16_t where)
{
	send_high(where);
	send_low(where);
}
