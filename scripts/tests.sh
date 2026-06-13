rm -rf bin
mkdir -p bin
gcc -std=gnu99 -Wall -Wextra -Werror -pedantic -Iinclude src/*.c -o bin/pico

BINDIR="bin"
NAME="pico"

BCYAN="\033[1;36m"
B_BL="\033[1;34m"
BPINK="\033[1;35m"
B_GN="\033[1;32m"
B_YL="\033[1;33m"
B_RD="\033[1;31m"
BOLD="\033[1m"
RESET="\033[0m"


echo -e "${BPINK}Exécution des tests :${RESET}"
echo -e "\n${BCYAN}1. Test d'addition  par redirection d'entrée :${RESET} 33 + 22 = ${B_GN}55${RESET}"
./${BINDIR}/${NAME} < exemples/addition.hex
echo -e "\n${BCYAN}2. Test d'addition  par tube :${RESET} 33 + 22 = ${B_GN}55${RESET}"
echo "600C49D049D140D060D148D241D23322" | ./${BINDIR}/${NAME}
echo -e "\n${BCYAN}3. Test de soustraction par tube :${RESET} 33 - 22 = ${B_GN}11${RESET}"
cat exemples/soustraction_11.hex | ./${BINDIR}/${NAME}
echo -e "\n${BCYAN}4. Test de soustraction par tube :${RESET} 255 - 1 = 254 ${B_GN}(FE)${RESET}"
{ cat exemples/soustraction_user.hex; echo -e "\nFF01"; } | ./${BINDIR}/${NAME}
echo -e "\n${BCYAN}5. Test ex13.9 par tube :${RESET} cube(6) = 216 ${B_GN}(D8)${RESET}"
printf "%s\n06\n" "$(cat exemples/ex13-9.hex)" | timeout 2 ./${BINDIR}/${NAME}
echo -e "\nFin des tests."
