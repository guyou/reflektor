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
#include "mirror.h"
#include "util.h"
#include "private.h"
#include "discover.h"

long long	bin_to_id	(char *data, unsigned char len) {
	long long	id;
	if(len > sizeof(id)) {
		fprintf(stderr,
			"Max id length %dB, but id reported %dB.\n",
			sizeof(id),len);
			}

	bcopy(data,&id,
		len>sizeof(id)?sizeof(id):len);
	ntohll(&id);
	return id;
}

void	mirror_send_event	(Mirror *m, VEvent *vh) {
	short			cmd;
	short			msgid;
	unsigned char	len;
	int				ofs;
	char			*buf;

	printf("Sending event:\n");
	vevent_print(vh);

	cmd				=	htons(vh->cmd);
	msgid			=	htons(vh->msgid);
	len				=	vh->len;

	buf	=	calloc(1+sizeof(cmd)+sizeof(msgid)+sizeof(len)+len,							1);

	buf[0]			=	0x01;
	ofs			=	1;
	bcopy(&cmd,buf+ofs,sizeof(cmd));
	ofs				+=	sizeof(cmd);
	bcopy(&msgid,buf+ofs,sizeof(msgid));
	ofs				+=	sizeof(msgid);
	bcopy(&len,buf+ofs,sizeof(len));
	if(len) {
		bcopy(vh->data,buf+ofs,len);
		ofs			+=	len;
	}
	print_hex(buf,ofs);
	fwrite(buf,1,ofs,m->fd);
	fflush(m->fd);
}

VEvent*	mirror_get_event	(Mirror *m) {
/*
 * this runs the event loop until an event is discovered, then
 * packages it for export.
 */
	VECmd	cmd		=	0;
	unsigned char	flag;
	VEvent	*ve		=	vevent_create();
	while( fread(&flag,sizeof(flag),1,m->fd) ){
		if(!flag) continue;
		cmd			=	flag;
		fread(&flag,1,1,m->fd);
		cmd			<<=	8;
		cmd			|=	flag;
		ve->cmd		=	cmd; // BUG: I don't think this works on BE sys.
		if( fread(&ve->msgid,2,1,m->fd) &&
			fread(&ve->len,1,1,m->fd)
			) {
				ve->msgid		=	ntohs(ve->msgid);
				if(ve->len) {
					ve->data	=	calloc(ve->len,1);
					fread(ve->data,1,ve->len,m->fd);
					if(	ve->cmd	== V_SHOW_TAG ||
						ve->cmd == V_HIDE_TAG ) {
							ve->id	=	bin_to_id(ve->data,ve->len);
						}
				}
		}
		return ve;
	}
	return (VEvent *)NULL;
}


FILE *	mirror_fdopen	(char *path) {
	FILE	*fd;
	if(!path) path = mirror_discover();
	while ( (fd = fopen(path,"r+") ) == NULL ) {
		printf("Mirror unavailable: trying again in ten seconds\n%s: %s\n",
			path,strerror(errno));
		sleep(VIOLET_SLEEPTIME);
	}
	return fd;
}

void	mirror_getid (Mirror *m) {
	VEvent	*id			=	vevent_build(V_GET_MIRRORID,
								m->msgid++,0,NULL);
	mirror_send_event(m,id);
	vevent_destroy(id);
}

void	mirror_getfwv (Mirror *m) {
	VEvent	*id			=	vevent_build(V_GET_APPLVERSION,
								m->msgid++,0,NULL);
	mirror_send_event(m,id);
	vevent_destroy(id);
}

void	mirror_getblv (Mirror *m) {
	VEvent	*id			=	vevent_build(V_GET_BOOTLOADERVERSION,
								m->msgid++,0,NULL);
	mirror_send_event(m,id);
	vevent_destroy(id);
}

void	mirror_setid (Mirror *m,unsigned char *buf) {
	long	id;
	bcopy(buf,&id,4);
	m->id	=	ntohl(id);
}

Mirror *	mirror_open (char *path) {
/*
 * The calloc() may cause problems with SELinux
 * after dropping root permissions.
 */

	FILE	*fh	=	mirror_fdopen(path);
	drop_root();
	
	Mirror	*m	=	mirror_create();
	m->path		=	path;
	m->fd		=	fh;
	mirror_getid(m);
	return m;
}

Mirror *	mirror_create (void) {
	return	calloc(1,sizeof(Mirror));
}



void	mirror_setstate	(Mirror *m, MState state) {
	m->state	=	state;
}

