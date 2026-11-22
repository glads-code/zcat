# zcat

`zcat` portable, le même sous Linux et sous Windows : décompresse des fichiers gzip (ou l'entrée standard) sur la sortie standard, comme `gzip -dc`. Windows ne fournit rien pour lire un `.gz` seul (son `tar.exe` ne lit le gzip qu'autour d'une archive tar), et 9pm non plus.

- `zcat.c` : le programme (licence 0BSD, `LICENSE`) (membres gzip enchaînés acceptés ; sortie 1 sur fichier tronqué, données corrompues ou erreur d'écriture, après avoir écrit la partie lisible, comme gzip).
- `zlib/` : fichiers de zlib 1.4 nécessaires à la décompression, sans modification (licence zlib, `zlib/LICENSE`).
- `essai.rc <binaire>` : fichier, entrée standard, membres enchaînés, refus d'un fichier tronqué et d'un fichier qui n'est pas du gzip (fichiers d'essai dans `essai/`).

## Installer sous Debian 13

    sudo apt install git tcc 9base
    git clone https://github.com/glads-code/zcat.git
    cd zcat
    ./compiler.rc

`./compiler.rc` compile avec tcc, lance l'essai et installe `/usr/local/bin/zcat` (devant le `zcat` de gzip dans le PATH, même usage).

## Installer sous Windows

Prérequis : tcc récent dans `C:\tcc-dev`, voir [Installer tcc sous Windows](https://github.com/glads-code/rcsh#installer-tcc-sous-windows) (README de rcsh) ; pour l'essai, 9pm et le rcsh corrigé (même README). Dans PowerShell :

```powershell
cd $env:USERPROFILE
curl.exe -Lo zcat.zip https://github.com/glads-code/zcat/archive/refs/heads/main.zip
tar -xf zcat.zip
cd zcat-main
C:\tcc-dev\tcc.exe -O2 -o zcat.exe zcat.c zlib\adler32.c zlib\crc32.c zlib\inffast.c zlib\inflate.c zlib\inftrees.c zlib\zutil.c
Copy-Item zcat.exe C:\9pm\bin\          # ou tout autre dossier du PATH
cmd /c "rcsh essai.rc C:/9pm/bin/zcat.exe < NUL"   # chemin complet : l'essai change de dossier
```

`zcat.exe` : 64 bits, sans DLL.

Sorties identiques à `gzip -dc` sur de gros jeux de données ouverts (Base adresse nationale, fichiers de plusieurs Go), sous Linux et sous Windows. Environ 1,5 fois plus lent que gzip (tcc optimise moins que gcc) : 37 s au lieu de 25 s pour la Base adresse nationale.

## Support

Dépôt publié tel quel, pour nos propres besoins : pas de support, les issues et pull requests ne sont pas suivies.
