Name:           tzcat
Version:        %{version}
Release:        1%{?dist}
Summary:        Décompresse des fichiers gzip sur la sortie standard
License:        0BSD AND Zlib
URL:            https://github.com/glads-code/zcat
Source0:        tzcat-%{version}.tar.gz
BuildRequires:  gcc
BuildRequires:  gzip

%description
Équivalent de « gzip -dc » (zlib incluse), le même sous Linux et
Windows ; nommé tzcat pour ne pas remplacer le zcat de gzip.
Publié sans support.

%prep
%autosetup

%build
gcc %{optflags} -o tzcat zcat.c zlib/adler32.c zlib/crc32.c zlib/inffast.c zlib/inflate.c zlib/inftrees.c zlib/zutil.c

%check
# Mêmes points que essai.rc (qui demande le rc de Plan 9, absent de Fedora)
cd essai
./../tzcat texte.gz | cmp - texte.txt
./../tzcat < texte.gz | cmp - texte.txt
./../tzcat deux.gz | cmp - <(gzip -dc deux.gz)
! ./../tzcat tronque.gz > /dev/null 2>&1
! ./../tzcat texte.txt > /dev/null 2>&1

%install
install -Dm755 tzcat %{buildroot}%{_bindir}/tzcat

%files
%license LICENSE zlib/LICENSE
%doc README.md
%{_bindir}/tzcat

%changelog
* Sun Nov 22 2026 GLADS <contact@glads.fr> - 1.0-1
- Première version empaquetée
