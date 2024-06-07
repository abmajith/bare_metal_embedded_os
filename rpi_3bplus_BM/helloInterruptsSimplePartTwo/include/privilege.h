

#ifndef PRIVILEGE_H
#define PRIVILEGE_H


/* finding the current execution level, 
	0 is the least privilege level (application level),
	1 is the operating system level,
	2 is the hypervisor level,
	3 is the top privilege level, secure firmware boot level 
*/

unsigned int get_current_el();


#endif // PRIVILEGE_H

