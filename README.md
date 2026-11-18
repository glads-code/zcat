# zcat

`zcat` des scripts du poste, le même sous Linux et sous Windows : décompresse des fichiers gzip (ou l'entrée standard) sur la sortie standard, comme `gzip -dc`. Windows ne fournit rien pour lire un `.gz` seul (son `tar.exe` ne lit le gzip qu'autour d'une archive tar), et 9pm non plus.

- `zcat.c` : le programme (membres gzip enchaînés acceptés ; sortie 1 sur fichier tronqué, données corrompues ou erreur d'écriture, après avoir écrit la partie lisible, comme gzip).
- `zlib/` : fichiers de zlib 1.4 nécessaires à la décompression, sans modification (licence zlib, `zlib/LICENSE`).
- `essai.rc <binaire>` : fichier, entrée standard, membres enchaînés, refus d'un fichier tronqué et d'un fichier qui n'est pas du gzip (fichiers d'essai dans `essai/`).

## Compiler

Linux (deb) : `./compiler.rc` compile avec tcc, lance l'essai et installe `/usr/local/bin/zcat` (devant le `zcat` de gzip dans le PATH, même usage).

Windows (tcc pour Windows dans `C:\tcc`) :

```
C:\tcc\tcc.exe -O2 -o C:\outils\zcat.exe zcat.c zlib\adler32.c zlib\crc32.c zlib\inffast.c zlib\inflate.c zlib\inftrees.c zlib\zutil.c
```

Sorties identiques à `gzip -dc` sur les données de dvf (BAN, contours, géolocalisation DVF), sous Linux et sous Windows. Environ 1,5 fois plus lent que gzip (tcc optimise moins que gcc) : 37 s au lieu de 25 s pour la BAN.
