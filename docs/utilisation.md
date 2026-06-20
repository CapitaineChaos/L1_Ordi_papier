

## Priorités

Lors de l'utilisation de toutes les données d'entrée, les priorités sont les suivantes de la plus haute à la plus basse :

1. Les options -a et -t
2. Le stdin (pipe, redirection, here-document, here-string)
3. L'option -f


## Mode décimal / hexadécimal

Par défaut les entrées clavier sont en décimal. La lecture dans les fichiers ou en entrée standard sont uniquement en hexadécimal.

Possibilité de changer le type de saisie en mode direct avec :
- option -x pour hexa
Ou en mode debogueur avec les raccourcis :
- commande x pour passer en hexadécimal
- commande d pour passer en décimal
La sortie est en décimal par défaut en mode standard et suit le mode d'entrée. En débogage la sortie est hexadécimale.

## Autres options

1. -n : pas de de retour entre les lignes
2. -p : conversion ASCII des caracteres imprimables en sortie

## Utilisation

### Chaîne de caractères

```bash
echo "80 09 49 32 41 32 49 33 41 33 FF" | bin/pico
```

### Pipe

```bash
cat exemples/simple.hex | bin/pico  | xxd -r -ps; echo;
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
bash
bin/pico <<< "801F004848704170006548704170006C48704170006C48704170006F48704170FF" | xxd -r -ps; echo;
```

```bash
bash
bin/pico <<< "801F004848704170006548704170006C48704170006C48704170006F48704170FF" -n -p
```

### Fichier en argument

```bash
bin/pico -d -f exemples/simple.hex
```
