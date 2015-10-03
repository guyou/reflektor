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

#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <asm/byteorder.h>
#include "reflektor.h"
#include "vevent.h"
#include "private.h"
#include "util.h"

int		export_tag (long long tag, char *arg) {
	static char cmd[1024];
	snprintf(cmd,1024,"%s %s %llx &",VIOLET_CMD_EXEC,arg,tag);
	return system(cmd);
}

void	report_mirror_id (Mirror *m) {
	printf("Mirror ID: %x\n",m->id);
}

void	report_mirror_state (Mirror *m) {
	switch(m->state) {
		case MIRROR_ORIENTATION_UP:
			printf("Mirror %x awake.\n",m->id);
			break;
		case MIRROR_ORIENTATION_DOWN:
			printf("Mirror %x asleep.\n",m->id);
			break;
		default:
			printf("Mirror %x state unknown (0x%x).\n",m->id,m->state);
		}
	}

void report_tag	(VEvent *vh) {
	switch(vh->cmd) {
		case V_SHOW_TAG:
			printf("SHOW %llx\n",vh->id);
			export_tag(vh->id,"show");
			break;
		case V_HIDE_TAG:
			printf("HIDE %llx\n",vh->id);
			export_tag(vh->id,"hide");
			break;
	}
}

int		main	(int argc, char *argv[]) {

	Mirror	*m;
	VEvent	*vh;

	print_privs();
	m				=	mirror_open(NULL);

	while( (vh = mirror_get_event(m)) ) {

		printf("Received event:\n");
		vevent_print(vh);
		printf("\n");

		switch(vh->cmd) {
			case V_MIRRORID:
				mirror_setid(m,vh->data);
				report_mirror_id(m);
				break;
			case V_ORIENTATION_UP:
			case V_ORIENTATION_DOWN:
				m->state	=	vh->cmd;
				report_mirror_state(m);
				break;
			case V_SHOW_TAG:
			case V_HIDE_TAG:
				report_tag(vh);
				break;
			case V_APPLVERSION:
			case V_CHOREO_DATA:
			case V_USB_UPDATE:
			case V_BOOTLOADERVERSION:
			default:
				break;
		}
	}

	printf("mir:ror disconnected\n");
	return 0;
}

