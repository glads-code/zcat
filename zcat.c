/* zcat : décompresse des fichiers gzip (ou l'entrée standard) sur la sortie
 * standard, comme gzip -dc. Membres gzip enchaînés acceptés. Sortie 1 sur
 * toute erreur (fichier tronqué, données corrompues, écriture impossible).
 * Même source sous Linux et Windows, compilée avec tcc et zlib (zlib/). */
#include <stdio.h>
#include <string.h>
#include "zlib/zlib.h"
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#define TAILLE 65536

static unsigned char ent[TAILLE], sor[TAILLE];

static int erreur(const char *nom, const char *msg)
{
	fprintf(stderr, "zcat: %s: %s\n", nom, msg);
	return 1;
}

/* décompresse f vers stdout ; 0 si tout va bien */
static int zcat(FILE *f, const char *nom)
{
	z_stream z;
	int r = Z_OK, fini = 0;
	size_t n;

	memset(&z, 0, sizeof z);
	/* 15 + 16 : en-tête gzip attendu */
	if (inflateInit2(&z, 15 + 16) != Z_OK)
		return erreur(nom, "inflateInit2");
	for (;;) {
		if (z.avail_in == 0) {
			n = fread(ent, 1, TAILLE, f);
			if (ferror(f)) { inflateEnd(&z); return erreur(nom, "lecture"); }
			if (n == 0) break;
			z.next_in = ent;
			z.avail_in = (uInt)n;
		}
		/* membre terminé et encore des données : membre suivant */
		if (fini) { inflateReset(&z); fini = 0; }
		do {
			z.next_out = sor;
			z.avail_out = TAILLE;
			r = inflate(&z, Z_NO_FLUSH);
			if (r != Z_OK && r != Z_STREAM_END && r != Z_BUF_ERROR) {
				inflateEnd(&z);
				return erreur(nom, z.msg ? z.msg : "données invalides");
			}
			n = TAILLE - z.avail_out;
			if (n && fwrite(sor, 1, n, stdout) != n) { inflateEnd(&z); return erreur(nom, "écriture"); }
			if (r == Z_STREAM_END) { fini = 1; break; }
		} while (z.avail_out == 0);
	}
	inflateEnd(&z);
	if (!fini) return erreur(nom, "fichier tronqué ou vide");
	return 0;
}

int main(int argc, char **argv)
{
	int i, e = 0;
	FILE *f;

#ifdef _WIN32
	_setmode(_fileno(stdin), _O_BINARY);
	_setmode(_fileno(stdout), _O_BINARY);
#endif
	if (argc < 2)
		e = zcat(stdin, "-");
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) { e |= zcat(stdin, "-"); continue; }
		if (!(f = fopen(argv[i], "rb"))) { e |= erreur(argv[i], "ouverture impossible"); continue; }
		e |= zcat(f, argv[i]);
		fclose(f);
	}
	if (fflush(stdout) != 0) e |= erreur("-", "écriture");
	return e;
}
