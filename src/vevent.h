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

#ifndef REFLEKTOR_VEVENT_H
#define REFLEKTOR_VEVENT_H

typedef	enum VEventType	{
	V_GET_MIRRORID			=	0x0101,
	V_MIRRORID				=	0x0102,
	V_GET_ORIENTATION		=	0x0103,
	V_ORIENTATION_UP		=	0x0104,
	V_ORIENTATION_DOWN		=	0x0105,
	V_SHOW_TAG				=	0x0201,
	V_HIDE_TAG				=	0x0202,
	V_SET_CHOREO_OFF		=	0x0301,
	V_PLAY_LAST_CHOREO		=	0x0303,
	V_SET_CHOREO_POWERONLY	=	0x0304,
	V_GET_CHOREO_DATA		=	0x0307,
	V_CHOREO_DATA			=	0x0308,
	V_USB_UPDATE			=	0x0401,
	V_GET_APPLVERSION		=	0x0404,
	V_APPLVERSION			=	0x0405,
	V_GET_BOOTLOADERVERSION	=	0x0406,
	V_BOOTLOADERVERSION		=	0x0407,
	V_ENTER_USB_UPDATE		=	0x0409,
	V_ERROR_1				=	0xff01,
	V_ERROR_2				=	0xff02,
	V_ERROR_3				=	0xff03
} VECmd;

typedef struct violet_event {
	VECmd			cmd;
	unsigned short	msgid;
	unsigned char	len;
	char			*data;
	unsigned long long	id;
} VEvent;

VEvent*	vevent_create		( void									);
VEvent*	vevent_build		( VECmd cmd,	short msgid,	
								unsigned char len,	char *data		);
void	vevent_destroy		( VEvent *vh							);
void	vevent_print		( VEvent *vh							);


#endif
