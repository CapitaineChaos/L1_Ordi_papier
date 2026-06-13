

## Priorités

Lors de l'utilisation de toutes les données d'entrée, les priorités sont les suivantes de la plus haute à la plus basse :

1. Les options -a et -t
2. Le stdin (pipe, redirection, here-document, here-string)
3. L'option -f


## Utilisation

### Chaîne de caractères

```bash
echo "80 09 49 32 41 32 49 33 41 33 FF" | bin/pico
```

### Pipe

```bash
cat exemples/simple.hex | bin/pico
```

### Redirection de l'entrée standard

```bash
bin/pico < exemples/simple.hex
```

### Concaténation stdin

```bash
{ cat exemples/simple.hex; echo "66676869"; } | bin/pico
```
### Here-document

```bash
bin/pico -d << EOF
; Programme de test qui demande une entrée utilisateur
; et l'affiche

80    ; Adresse de début du programme
09     ; Taille du programme (9 octets)

; Instructions du programme :
49     ; IN @ 32 - Lire une entrée et la stocker à l'adresse 32
32     ; adresse 32
41     ; OUT @ 32 - Afficher le contenu de l'adresse 32
32     ; adresse 32
49     ; IN @ 33 - Lire une autre entrée et la stocker à l'adresse 33
33     ; adresse 33
41     ; OUT @ 33 - Afficher le contenu de l'adresse 33
33     ; adresse 33
FF     ; STOP / ERREUR
EOF
```

### Here-string

```bash
bin/pico -d <<< "80 09 49 32 41 32 49 33 41 33 FF"
```

### Fichier en argument

```bash
bin/pico -d -f exemples/simple.hex
```
