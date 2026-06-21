#!/bin/bash

# Tester toutes les divisions de 0 à 255
# Comparer les résultats avec le calcul de bash
# Afficher "o" ou "x" en fonction du résultat
# 64 marques (4*16) par ligne, un bloc par dividende,
# 2 sauts de ligne entre les blocs, entête = la division comparée

# Programme de division entière (Q = a / b) chargé une seule fois.
# a et b sont fournis en hexa à la fin du buffer, lus par les deux IN.
PROG=$(cat <<'PROGEOF'
70 ; ADREESSE DEBUT
27 ; LONGUEUR
; adr 70
00 00    ; LOAD 00
; adr 72
48 9C    ; STORE a

; adr 74
49 9A    ; IN x
; 76
49 9B    ; IN y

; 78
40 9B  ; LOAD y

; 7A

48   9D   ; store b

; 7C
40 9D ; LOAD b

; 7E
11 8E    ; BRN aller vers adr:  ??

; 80
40 9D   ; load b
; 82
61 9A ; sub x  (b-x)
; 84
48 9D ; store dans b
; 86
40 9C  ; load a
; 88
20 01 ; add a
; 8A
48 9C  ; store a 
; 8C
10 7C  ; JUMP 7C
;8E
40 9C  ; LOAD a


;90 

21 01 ; sub 01 a

;92

48 9C ; store a

;94
41 9C  ; out a


FF
;9A    variable x = j
;9B    variable y = k
;9C    variable a = R
;9D variable b = b
PROGEOF
)
# Deux niveaux de parallélisation, chacun avec son propre plafond :
#   NPROC_BLOCS : nombre de blocs (dividendes) traités en même temps   (niveau 1)
#   NPROC_DIV   : dans un bloc, nombre de divisions en même temps        (niveau 2)
NPROC=$(nproc 2>/dev/null || echo 4)
NPROC_BLOCS=$NPROC
NPROC_DIV=$NPROC

# Teste UNE division (i,j) et écrit 'o' ou 'x' dans dir/d_iii_jjj (indexé pour l'ordre).
# o = pico == bash, x = divergence.
une_div() {
    local i=$1 j=$2 dir=$3 output
    output=$(printf '%s\n%02X %02X\n' "$PROG" "$j" "$i" | timeout 5 ./bin/pico 2>/dev/null)
    if [ "$output" = "$((i / j))" ]; then
        printf 'o' > "$dir/d_$(printf '%03d_%03d' "$i" "$j")"
    else
        printf 'x' > "$dir/d_$(printf '%03d_%03d' "$i" "$j")"
    fi
}

# NIVEAU 2 : un bloc (dividende i) ; lance un sous-shell par division (plafond NPROC_DIV),
# puis assemble le bloc fini dans dir/b_iii.
bloc() {
    local i=$1 dir=$2 ii j running=0
    ii=$(printf '%03d' "$i")
    for j in {1..255}
    do
        une_div "$i" "$j" "$dir" &
        running=$((running + 1))
        if [ "$running" -ge "$NPROC_DIV" ]; then
            wait -n
            running=$((running - 1))
        fi
    done
    wait                       # toutes les divisions du bloc
    {
        printf '=== %d / j ===\n' "$i"
        printf -- '-%s\n' "$(cat "$dir/d_${ii}_"* 2>/dev/null)" | fold -w128
        printf '\n'
    } > "$dir/b_$ii"
    printf '.' >&2             # progression : un bloc vient d'être calculé
}

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

# NIVEAU 1 : un sous-shell par bloc (plafond NPROC_BLOCS).
# Chaque "bloc ... &" fork le shell (hérite de PROG, bloc, une_div) et lance lui-même
# ses propres divisions en parallèle -> deux niveaux de parallélisation imbriqués.
running=0
for i in {0..255}
do
    bloc "$i" "$tmpdir" &
    running=$((running + 1))
    if [ "$running" -ge "$NPROC_BLOCS" ]; then
        wait -n
        running=$((running - 1))
    fi
done
wait                           # tous les blocs restants
printf '\n' >&2                # fin de la barre de progression

# Affichage ordonné des blocs ; croix x en rouge (couleur après le fold).
cat "$tmpdir"/b_* | sed $'s/x/\033[31mx\033[0m/g'
