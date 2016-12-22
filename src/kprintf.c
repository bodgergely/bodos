#include "kprintf.h"

typedef enum arg_type
{
	CHAR,
	INT,
	UNSIGNED_INT,
	POINTER,
	DOUBLE,
	STRING
}arg_type;


static int is_formatter(const char* c)
{
	if(*c == '%' && *(c+1) && !isspace(*(c+1)))
		return 1;
	else return 0;
}

/*
static int count_args(const char* format)
{
	int count = 0;
	if(!*format)
		return 0;

	for(const char* s=format;*(s+1);s++)
	{
		if(is_formatter(s))
		{
			++count;
			++s;
		}
	}

	return count;
}
*/

static arg_type search_next_arg(const char* search_from, const char** arg_start, const char** next_from)
{
	*next_from = NULL;
	*arg_start = NULL;
	arg_type type = INT;
	for(const char* s = search_from;*s;s++)
	{
		if(is_formatter(s))
		{
			*arg_start = s;
			switch(*(s+1))
			{
			case 'c':
				type = CHAR;
				break;
			case 'd':
				if(*(s+2) == 'u')
				{
					type = UNSIGNED_INT;
				}
				else
				{
					type = INT;
				}
				break;
			case 'p':
				type = POINTER;
				break;
			case 'f':
				type = DOUBLE;
				break;
			case 's':
				type = STRING;
				break;
			}

			if(type == UNSIGNED_INT)
				*next_from = *arg_start + 3;
			else
				*next_from = *arg_start + 2;

		}
		if(*arg_start!=NULL)
		{
			break;
		}
	}

	return type;
}

static int write_gen(const char* data, size_t size, log_target target)
{
	if(target == SERIAL_PORT)
		serial_write(data, size);
	else if(target == FRAME_BUFFER)
		terminal_write(data, size);

	return size;
}

static void create_log_level_string(log_level level, char* buff)
{
	switch(level)
	{
	case TRACE:
		strcpy(buff, "TRACE ");
		break;
	case INFO:
		strcpy(buff, "INFO ");
		break;
	case WARN:
		strcpy(buff, "WARN ");
		break;
	case ERR:
		strcpy(buff, "ERR ");
		break;
	case FATAL:
		strcpy(buff, "FATAL ");
		break;
	}
}


int kprintf(log_level lev, log_target target, const char *format, ...)
{
	const size_t arg_container_size = 128;
	char arg_container[arg_container_size];
	char loglevel[8];
	int written = 0;
	size_t len = strlen(format);

	create_log_level_string(lev, loglevel);
	written +=  write_gen(loglevel, strlen(loglevel), target);
	arg_container[0] = '\0';

	va_list a_list;
	va_start( a_list, format );

	char val_char;
	const char* val_string = NULL;
	int val_int;
	unsigned int val_unsigned;
	void* val_pointer;
	const char* search_from = format;
	const char* arg_start = format + len;
	const char* next_from = NULL;

	for(;;)
	{
		arg_type type = search_next_arg(search_from, &arg_start, &next_from);
		if(arg_start!=NULL)
		{
			switch(type)
			{
			case CHAR:
				val_char = (char)va_arg(a_list, int);
				arg_container[0] = val_char;
				arg_container[1] = '\0';
				break;
			case INT:
				val_int = va_arg(a_list,int);
				itoa(val_int, arg_container, 10);
				break;
			case UNSIGNED_INT:
				val_unsigned = va_arg(a_list, unsigned int);
				itoa(val_unsigned, arg_container, 10);
				break;
			case POINTER:
				val_pointer = (void*)va_arg(a_list, unsigned int);
				itoa((unsigned int)val_pointer, arg_container, 10);
				break;
			case DOUBLE:
				//TODO
				break;
			case STRING:
				val_string = va_arg(a_list, const char*);
				strcpy(arg_container, val_string);
				break;
			}
		}
		else
			arg_start = format + len;

		written += write_gen(search_from, arg_start-search_from, target);

		if(arg_start && arg_start!=(format+len))
		{
			written += write_gen(arg_container, strlen(arg_container), target);
		}

		search_from = next_from;
		arg_start = NULL;
		if(!search_from)
			break;
	}

	return written;

}
