# -*- Mode: Makefile -*-
#
# Makefile.am - top level automake file for Redland
#
# Copyright (C) 2000-2009 David Beckett http://www.dajobe.org/
# Copyright (C) 2000-2005 University of Bristol, UK http://www.bristol.ac.uk/
# 
# This package is Free Software and part of Redland http://librdf.org/
# 
# It is licensed under the following three licenses as alternatives:
#   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
#   2. GNU General Public License (GPL) V2 or any newer version
#   3. Apache License, V2.0 or any newer version
# 
# You may not use this file except in compliance with at least one of
# the above three licenses.
# 
# See LICENSE.html or LICENSE.txt at the top of this package for the
# complete terms and further detail along with the license texts for
# the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
# 
# 

bin_SCRIPTS = redland-config
noinst_SCRIPTS = redland-src-config

man_MANS = redland-config.1

dist_pkgdata_DATA = Redland.i

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = redland.pc

DISTCHECK_CONFIGURE_FLAGS=--enable-gtk-doc

# Subdirectories to docs-build/install/distribute etc.
SUBDIRS=$(subdirs) src examples utils demos docs data scripts

EXTRA_DIST=\
ChangeLog.1 ChangeLog.2 ChangeLog.3 ChangeLog.4 \
ChangeLog.5 ChangeLog.6 ChangeLog.7 ChangeLog.8 ChangeLog.9 ChangeLog.10 \
ChangeLog.11 ChangeLog.12 \
README                LICENSE.txt  TODO \
README.html NEWS.html LICENSE.html TODO.html \
RELEASE.html INSTALL.html FAQS.html \
LICENSE-2.0.txt NOTICE \
HACKING.md HACKING.html \
redland.rdf.in \
autogen.sh \
redland-config.in \
redland-src-config.in \
$(man_MANS) \
redland.sln \
gtk-doc.make \
redland.pc.in

if MAINTAINER_MODE
# Create some text files from HTML sources
LYNX=lynx

HTML_TO_TEXT=TERM=vt100 $(LYNX) -dump $< | sed -e 's,file://localhost.*librdf/,,' > $@

SUFFIXES = .html .txt

.html.txt:
	$(HTML_TO_TEXT)

README: README.html
	$(HTML_TO_TEXT)

NEWS: NEWS.html
	$(HTML_TO_TEXT)

TODO: TODO.html
	$(HTML_TO_TEXT)

HACKING.html: HACKING.md $(top_srcdir)/scripts/markdown-to-html.pl
	$(PERL) $(top_srcdir)/scripts/markdown-to-html.pl $<  $@

endif

@SET_MAKE@

install-data-hook:
	$(PERL) $(srcdir)/scripts/touch-mtime.pl $(srcdir)/Redland.i $(DESTDIR)$(pkgdatadir)/Redland.i
