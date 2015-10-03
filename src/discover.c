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

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/hiddev.h>
#include <linux/input.h>
#include <string.h>
#include <glob.h>
#include "discover.h"

#define HIDIOCGRAWINFO          _IOR('H', 0x03, struct hidraw_devinfo)

struct hidraw_devinfo {
	__u32 bustype;
	__s16 vendor;
	__s16 product;
};

void	glob_devices	(glob_t * globbuf) {
	if(glob("/dev/hidraw*", 0, NULL, globbuf) != 0) {
		printf("DEBUG: glob() failed.\n");
	}
}

char *	mirror_discover	(void) {
	int fd;
	int len;
	struct hidraw_devinfo hrdi;
	glob_t globbuf;
	unsigned int i;
	char	*path	=	NULL;
	

	glob_devices(&globbuf);
	for(i=0;i<globbuf.gl_pathc;i++) {
		if((fd=open(globbuf.gl_pathv[i], O_RDONLY)) < 0) {
			printf("error while opening %s.\n", globbuf.gl_pathv[i]);
			continue;
		}
		ioctl(fd,HIDIOCGRAWINFO,&hrdi);
		if(hrdi.vendor == 0x1da8 && hrdi.product == 0x1301) {
			printf("Found device: %s\n", globbuf.gl_pathv[i]);
			path		=	strdup(globbuf.gl_pathv[i]);
			}
		close(fd);
	}
	return path;
}


