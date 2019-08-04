/* Using LIN communication library as a master. */

#include "LIN_MASTER.h"

int main()
 { 
	
	init_lin_master(19200);
	
	while(1)
		{
			/* DO NOTHING */
		}

	return 0;	/* NOT REACHABLE */
 }

