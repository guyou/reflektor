/*
  This file is part of reflek:tor and libreflektor.

    reflek:tor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with reflek:tor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "vevent.h"

VEvent*	vevent_create		(void) {
/*
 * generic calloc()-based object creator
 * 
 */

	return calloc(1,sizeof(VEvent));
}

VEvent *	vevent_build	(VECmd cmd, short msgid, unsigned char len,
							char *data) {
	VEvent	*vh		=	vevent_create();
	vh->cmd			=	cmd;
	vh->msgid		=	msgid;
	vh->len			=	len;
	if(len) {
		vh->data	=	calloc(len,1);
		bcopy(data,vh->data,len);
	}
	return	vh;
}
							


void	vevent_destroy		(VEvent *vh) {
/*
 * free() vh->data if necessary
 */

	if(vh->data) {
		free(vh->data);
	}

	free(vh);
}

void	vevent_print		(VEvent *vh) {
	printf("\tcmd:\t%04x\n\tmsgid:\t%04x\n\tlen:\t%02x\n",
		vh->cmd,vh->msgid,vh->len);
	if(vh->len) {
		printf("\tdata:\t");
		int i;
		for(i=0; i<vh->len; i++) {
			printf("%02hhx%c",vh->data[i],i==vh->len-1?'\n':':');
		}
	}
	
	if(vh->id) {
		printf("\tid:\t%llx\n",vh->id);
	}
}



