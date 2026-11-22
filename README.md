# zcat

`zcat` portable, le même sous Linux et sous Windows : décompresse des fichiers gzip (ou l'entrée standard) sur la sortie standard, comme `gzip -dc`. Windows ne fournit rien pour lire un `.gz` seul (son `tar.exe` ne lit le gzip qu'autour d'une archive tar), et 9pm non plus.

- `zcat.c` : le programme (licence 0BSD, `LICENSE`) (membres gzip enchaînés acceptés ; sortie 1 sur fichier tronqué, données corrompues ou erreur d'écriture, après avoir écrit la partie lisible, comme gzip).
- `zlib/` : fichiers de zlib 1.4 nécessaires à la décompression, sans modification (licence zlib, `zlib/LICENSE`).
- `essai.rc <binaire>` : vérifie un exécutable de zcat. `<binaire>` est le chemin complet du zcat à essayer (par exemple `./essai.rc /usr/local/bin/zcat`), complet car l'essai se place dans `essai/`, où sont ses fichiers gzip. Il vérifie que zcat décompresse un fichier donné en argument et l'entrée standard, qu'il enchaîne les membres d'un fichier fait de plusieurs gzip mis bout à bout (comme `cat a.gz b.gz`), et qu'il échoue (code de sortie 1) sur un fichier tronqué ou qui n'est pas du gzip. Il affiche `essai réussi`, ou le point en échec.

## Installer sous Debian 13

    sudo apt install git tcc 9base
    git clone https://github.com/glads-code/zcat.git
    cd zcat
    ./compiler.rc

`./compiler.rc` compile avec tcc, lance l'essai et installe `/usr/local/bin/zcat` (devant le `zcat` de gzip dans le PATH, même usage).

## Installer sous Windows

Binaire prêt à l'emploi : `zcat.exe` dans les [Releases](https://github.com/glads-code/zcat/releases) (compilé sous Windows 11 selon la procédure ci-dessous, empreinte SHA-256 dans les notes de version). Le copier dans un dossier du `PATH` suffit. Pour le compiler soi-même :

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
