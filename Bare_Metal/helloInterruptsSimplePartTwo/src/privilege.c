#include "privilege.h"



unsigned int get_current_el()
{
	unsigned int el;
	asm volatile ("mrs %0, CurrentEL" : "=r"(el));
	return (el >> 2) & 3;
}
