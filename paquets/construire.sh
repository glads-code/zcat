#!/bin/sh
# Construit le paquet tzcat (zcat de ce dépôt, renommé pour ne pas
# entrer en conflit avec le zcat de gzip) dans un conteneur jetable,
# lancé par paquets.rc : .deb sous Debian, .rpm sous Fedora. Source en
# lecture seule dans /src, paquet écrit dans /dist.
# Usage : construire.sh deb|rpm <version>
set -eu
type=$1 version=$2
cp -r /src /tmp/zcat && cd /tmp/zcat
case $type in
deb)
	apt-get update -qq && apt-get install -y -qq gcc libc6-dev 9base >/dev/null
	gcc -O2 -o tzcat zcat.c zlib/adler32.c zlib/crc32.c zlib/inffast.c zlib/inflate.c zlib/inftrees.c zlib/zutil.c
	PATH=/usr/lib/plan9/bin:$PATH ./essai.rc $PWD/tzcat
	p=/tmp/p/tzcat_${version}-1_amd64
	install -Dm755 tzcat $p/usr/bin/tzcat
	{ cat LICENSE; echo; echo 'zlib/ :'; cat zlib/LICENSE; } > copyright
	install -Dm644 copyright $p/usr/share/doc/tzcat/copyright
	install -Dm644 README.md $p/usr/share/doc/tzcat/README.md
	mkdir -p $p/DEBIAN
	cat > $p/DEBIAN/control <<FIN
Package: tzcat
Version: ${version}-1
Architecture: amd64
Maintainer: GLADS <contact@glads.fr>
Depends: libc6 (>= 2.34)
Section: utils
Priority: optional
Homepage: https://github.com/glads-code/zcat
Description: décompresse des fichiers gzip sur la sortie standard
 Équivalent de « gzip -dc » (zlib incluse), le même sous Linux et
 Windows ; nommé tzcat pour ne pas remplacer le zcat de gzip.
 Publié sans support.
FIN
	dpkg-deb --root-owner-group --build $p /dist/ >/dev/null
	;;
rpm)
	dnf -q -y install gcc glibc-devel rpm-build gzip >/dev/null
	mkdir -p ~/rpmbuild/SOURCES
	tar -C /tmp -czf ~/rpmbuild/SOURCES/tzcat-$version.tar.gz --transform "s,^zcat,tzcat-$version," zcat
	rpmbuild -bb --quiet --define "version $version" paquets/tzcat.spec
	cp ~/rpmbuild/RPMS/x86_64/tzcat-$version-*.rpm /dist/
	;;
esac
