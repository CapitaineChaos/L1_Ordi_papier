
```text
PC : Program Counter ; adresse mémoire de la prochaine instruction
A  : Registre A
V  : Valeur donnée immédiatement

- La flèche ← représente l’affectation.
- (α) représente le contenu de la case d’adresse alpha.
- *(α) représente le contenu de la case dont l’adresse est dans la case d’adresse alpha.
- ¬ représente le non logique.
- & représente le et logique.
- Les crochets [ ] groupent les termes sur lesquels s’applique une opération.

#  est un adressage immédiat
α  est un adressage absolu
*α est un adressage indirect
```


```text
Mnémoniques     Code opératoire         description

Arithmétique
ADD #           20                      A ← A + V
ADD α           60                      A ← A + (α)
ADD *α          E0                      A ← A + *(α)
SUB #           21                      A ← A − V
SUB α           61                      A ← A − (α)
SUB *α          E1                      A ← A − *(α)

Logique
NAND #          22                      A ← ¬[A&V]
NAND α          62                      A ← ¬[A&(α)]
NAND *α         E2                      A ← ¬[A& *(α)]

Transferts
LOAD #          00                      A ← V
LOAD α          40                      A ← (α)
LOAD *α         C0                      A ← *(α)
STORE α         48                      (α) ← A
STORE *α        C8                      *(α) ← A

E/S
IN α            49                      (α) ← Entrée
IN *α           C9                      *(α) ← Entrée
OUT α           41                      Sortie ← (α)
OUT *α          C1                      Sortie ← *(α)

Branchement inconditionnel
JUMP α          10                      PC ← α

Branchements conditionnels
BRN α           11                      si A < 0 alors PC ← α
BRZ α           12                      si A = 0 alors PC ← α

```
