# -*- RPM-SPEC -*-
%define name    raptor2
%define version 2.0.15
%define release SNAP

Summary:   Raptor RDF Parser Toolkit for Redland
Name:      %{name}
Version:   %{version}
Release:   %{release}
Prefix:    %{_prefix}
License:   LGPLv2+ or ASL 2.0
Group:     Development/Libraries
Source:    http://download.librdf.org/source/%{name}-%{version}.tar.gz
URL:       http://librdf.org/raptor/
BuildRoot: /tmp/%{name}-%{version}
BuildRequires: libxml2 libxml2-devel >= 2.6.8
BuildRequires: curl curl-devel
BuildRequires: libxslt libxslt-devel
Packager:  Dave Beckett <dave@dajobe.org>
Docdir: %{_docdir}

%description

Raptor is the RDF Parser Toolkit for Redland that provides a set of
Resource Description Framework (RDF) parsers and serializers,
generating RDF triples from the following syntaxes: RDF/XML,
N-Triples, TRiG, Turtle, RSS tag soup including all versions of RSS,
Atom 1.0 and 0.3, GRDDL and microformats for HTML, XHTML and XML. The
serializing RDF triples to syntaxes are: RDF/XML, RSS 1.0, Atom 1.0,
N-Triples, XMP, Turtle, GraphViz DOT and JSON.

%package devel
Summary: Libraries, includes etc to develop with Raptor RDF parser library
Group: Development/Libraries
Requires: raptor = %{version}
Requires: libxml2-devel

%description devel
Libraries, includes etc to develop with Raptor RDF parser and
serializer library.

%prep
%setup -q

%build

%configure --enable-release

%{__make} OPTIMIZE="$RPM_OPT_FLAGS"

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

install -d $RPM_BUILD_ROOT%{_mandir}/man1
install -d $RPM_BUILD_ROOT%{_mandir}/man3

%makeinstall

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc AUTHORS COPYING COPYING.LIB ChangeLog LICENSE.txt NEWS README
%doc LICENSE-2.0.txt NOTICE
%doc *.html

%doc %{_mandir}/man1/rapper.1*
%doc %{_mandir}/man3/libraptor2.3*

%doc %{_datadir}/gtk-doc/html/raptor2/*

%{_libdir}/libraptor*.so.*
%{prefix}/bin/rapper


%files devel
%defattr(-, root, root)

%doc AUTHORS COPYING COPYING.LIB ChangeLog LICENSE.txt NEWS README
%doc LICENSE-2.0.txt NOTICE

%{_libdir}/libraptor*.so
%{_libdir}/pkgconfig/raptor2.pc

%{prefix}/include/raptor/*


%changelog
* Fri Jan 5 2007  Dave Beckett <dave@dajobe.org>
- rename files for raptor 2.0.0
- no more raptor-config
- do not package libraptor*.la or libraptor*.a files
- includes are in a subdir

* Fri Jan 5 2007  Dave Beckett <dave@dajobe.org>
- Document Turtle and DOT serializers

* Wed Feb 15 2006  Dave Beckett <dave@dajobe.org>
- Add libxslt and libxslt-devel for GRDDL

* Wed Dec 20 2005  Dave Beckett <dave@dajobe.org>
- Update description of parsers and serializers.

* Wed Nov 9 2005  Dave Beckett <dave@dajobe.org>
- Add gtk-doc documentation

* Wed Aug 11 2005  Dave Beckett <dave.beckett@bristol.ac.uk>
- Update Source:
- Use %makeinstall

* Wed Aug 10 2005  Dave Beckett <dave.beckett@bristol.ac.uk>
- Use %configure and %{_make}

* Thu Sep 9 2004 Dave Beckett <Dave.Beckett@bristol.ac.uk>
- License now LGPL/Apache 2
- Added LICENSE-2.0.txt and NOTICE

* Tue May 11 2004 Dave Beckett <Dave.Beckett@bristol.ac.uk>
- Added RELEASE.html

* Thu Apr 17 2003 Dave Beckett <Dave.Beckett@bristol.ac.uk>
- Added pkgconfig raptor.pc, raptor-config
- Requires curl

* Mon Jan 13 2003 Dave Beckett <Dave.Beckett@bristol.ac.uk>
- rdfdump now rapper

* Fri Dec 20 2002 Dave Beckett <Dave.Beckett@bristol.ac.uk>
- Updated to have two RPMs for raptor and raptor-devel.  Depend on
  libxml2 as XML parser.
