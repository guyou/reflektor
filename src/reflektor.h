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

#include "mirror.h"
#include "vevent.h"

#define	VIOLET_FLAG_STAT	0x01
#define	VIOLET_FLAG_DATA	0x02
#define VIOLET_FLAG_ERROR	0xff
#define	VIOLET_STAT_SHOW	0x01
#define	VIOLET_STAT_HIDE	0x02
#define	VIOLET_STAT_SLEEP	0x05
#define	VIOLET_STAT_AWAKE	0x04
#define	VIOLET_STAT_ID		0x02

int		export_tag			( long long tag,	char *arg			);
void	report_mirror_state	( Mirror *m								);
void	report_mirror_id	( Mirror *m								);
void	report_tag			( VEvent *vh							);
