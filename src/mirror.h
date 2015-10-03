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

#ifndef REFLEKTOR_MIRROR_H
#define REFLEKTOR_MIRROR_H

#include "vevent.h"

#define	VIOLET_SLEEPTIME	10

typedef	enum MirrorState {
	MIRROR_ORIENTATION_DOWN	=	V_ORIENTATION_DOWN,
	MIRROR_ORIENTATION_UP	=	V_ORIENTATION_UP
} MState;

typedef	struct	violet_mirror {
	int		id;
	char	*path;
	FILE	*fd;
	unsigned short	msgid;
	MState	state;
} Mirror;

VEvent*	mirror_get_event	( Mirror *m								);
void	mirror_send_event	( Mirror *m, VEvent *vh					);

Mirror*	mirror_create		( void									);
Mirror*	mirror_open			( char *path							);
void	mirror_setid		( Mirror *m,	unsigned char *buf		);
void	mirror_getid		( Mirror *m								);
void	mirror_getblv		( Mirror *m								);
void	mirror_getfwv		( Mirror *m								);
FILE*	mirror_fdopen		( char *path							);
void	mirror_setstate		( Mirror *m,	MState state			);

long long	bin_to_id		( char *data,	unsigned char len		);

#endif
