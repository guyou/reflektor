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
#include <poll.h>

#define TRUE             1
#define FALSE            0

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

void report (Mirror	*m, VEvent	*vh) {
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

VEvent *decode (Mirror *m, int fd) {
	VEvent *ve = vevent_create();
	scanf("%hx", &(ve->cmd));
	ve->msgid = m->msgid++;

	printf("Sending command:\n");
	vevent_print(ve);
	printf("\n");

	return ve;
}

#define NFDS 2

int		main	(int argc, char *argv[]) {

	Mirror	*m;
	VEvent	*vh;
	struct pollfd fds[NFDS];
	int timeout;
	int rc;
	int i;

	print_privs();
	m				=	mirror_open(NULL);

	/*************************************************************/
	/* Initialize the pollfd structure                           */
	/*************************************************************/
	memset(fds, 0 , sizeof(fds));

	fds[0].fd = 0;
	fds[0].events = POLLIN;

	fds[1].fd = fileno(m->fd);
	fds[1].events = POLLIN;

  /*************************************************************/
  /* Initialize the timeout to 3 minutes. If no                */
  /* activity after 3 minutes this program will end.           */
  /* timeout value is based on milliseconds.                   */
  /*************************************************************/
  timeout = (3 * 60 * 1000);
  timeout = -1;

do
  {
    /***********************************************************/
    /* Call poll() and wait 3 minutes for it to complete.      */
    /***********************************************************/
    for (i = 0; i < NFDS; i++)
      printf("Before: fds[%d]: %d, %20x, %20x\n", i, fds[i].fd, fds[i].events, fds[i].revents);
    printf("Waiting on poll()...\n");
    rc = poll(fds, NFDS, timeout);
    for (i = 0; i < NFDS; i++)
      printf("After: fds[%d]: %d, %20x, %20x\n", i, fds[i].fd, fds[i].events, fds[i].revents);

    /***********************************************************/
    /* Check to see if the poll call failed.                   */
    /***********************************************************/
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }

    /***********************************************************/
    /* Check to see if the 3 minute time out expired.          */
    /***********************************************************/
    if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
    }

    /***********************************************************/
    /* One or more descriptors are readable.  Need to          */
    /* determine which ones they are.                          */
    /***********************************************************/
    for (i = 0; i < NFDS; i++)
    {
      /*********************************************************/
      /* Loop through to find the descriptors that returned    */
      /* POLLIN and determine whether it's the listening       */
      /* or the active connection.                             */
      /*********************************************************/
      if(fds[i].revents == 0)
        continue;

      /*********************************************************/
      /* If revents is not POLLIN, it's an unexpected result,  */
      /* log and end the server.                               */
      /*********************************************************/
      if(fds[i].revents != POLLIN)
      {
        printf("  Error! revents = %d\n", fds[i].revents);
	fds[i].fd = -1;
        break;

      }
      if (fds[i].fd == 0)
      {
        VEvent *vh = decode(m, 0);
        if (vh)
          mirror_send_event(m, vh);
      }
      if (fds[i].fd == fileno(m->fd))
      {
        vh = mirror_get_event(m);
        report(m, vh);
      }
      
      fds[i].revents = 0;
    }
} while (TRUE);

	printf("mir:ror disconnected\n");
	return 0;
}

