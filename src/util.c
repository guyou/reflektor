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

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int		drop_root	(void) {
	if( setuid(getuid()) || setgid(getgid()) ) {
		perror("Could not drop root privileges.\n");
		return -1;
	}
	return 0;
}

void	print_privs	(void) {
	printf("UID: %d (%d)\nGID: %d (%d)\n",
		getuid(),geteuid(),getgid(),getegid());
}

void	print_hex	(char *buf, int len) {
	int i;
	for(i=0; i<len; i++) {
		printf("%02hhx%c",buf[i],i==len-1?'\n':':');
		}
}
