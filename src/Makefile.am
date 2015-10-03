
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#       
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.




bin_PROGRAMS = reflektor
bin_SUIDS = reflektor

reflektor_SOURCES = reflektor.c reflektor.h mirror.c mirror.h \
	private.h vevent.c vevent.h util.c util.h discover.c discover.h

install-suid-programs:
	@foo='$(bin_SUIDS)'; \
	for file in $$foo; do \
	x=$(DESTDIR)$(bindir)/$$file; \
	if chown 0:0 $$x && chmod u+s $$x; then :; else \
	echo "*"; \
	echo "* Failed to install $$x setuid root"; \
	echo "*"; \
	fi; done

install-exec-hook: install-suid-programs


