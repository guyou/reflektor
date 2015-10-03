bin_SCRIPTS = reflektor_do
SUBDIRS = src
EXTRA_DIST = reflektor_do autogen.sh

md5s	=	${distdir}.tar.gz.md5 ${distdir}.tar.bz2.md5
sigs	=	${distdir}.tar.gz.asc ${distdir}.tar.bz2.asc

%.asc: %
	gpg --armor --detach-sign -o $@ $<

%.md5: %
	md5sum $< > $@

release: dist distcheck
	make ${md5s}
