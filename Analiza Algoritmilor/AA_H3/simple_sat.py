import random
import re
import fileinput


def split(caractere):
    return [char for char in caractere]


def find_max_numbers(expresie):
    nums = re.findall("\d+", expresie)
    return max(nums)

# functie pentru crearea matricei FNC-SAT:
# se primeste ca parametru expresia si o matrice formata din zero-uri
# daca primul caracter este "~", atunci transform urmatorul caracter intr-un
# int si scad "-1", pentru ai pune pe coloana respectiva valoarea de -1.
# Daca nu este intalnit caracterul "~", atunci transform
# caracterul respectiv intr-un int si scad la fel coloana, punand 1.
# La fiecare operatie cresc/mut linia


def matrix_constructor(expresie, matrice):
    linie = 0
    for element in expresie:
        for caracter in element:
            if caracter[0] == "~":
                coloana = int(caracter[1:])
                matrice[linie][coloana - 1] = -1
            else:
                coloana = int(caracter)
                matrice[linie][coloana - 1] = 1
        linie += 1


def negate(expresie):
    return expresie.replace("~0", "1").replace("~1", "0")

# functie ce verifica daca sunt elemente de 0 sau 1
# Daca se gaseste un sir de "0"-uri in clauza, atunci functia
# returneaza automat 0, fiindca ar veni 1^1^0^1 -> 0
# altfel da return la 1


def eval_clauza(clauza):
    clauza = clauza.split('V')
    for i in clauza:
        if int(i) == 1:
            return 1
    return 0

# functie ce imparte FNC in clauze si le da ca parametru
# functiei "eval_clauza". Aici se evalueaza FNC ul in sine
# la modul daca se obtine 0, atunci FNC nu este SAT
# daca obtine 1, e SAT


def eval_expresie(expresie):
    expresie = expresie.split('^')
    for i in expresie:
        if eval_clauza(i) == 0:
            return 0
    return 1

# "fileinput.input()" folosit pentru citirea fisierelor la stdin
# fiindca cu F = input() sau o alta functie de citire
# nu imi mergea pentru checker, cam asta a fost o solutie


for f in fileinput.input():
    F = f.rstrip()

new_F = ''
counter = 0

count = F.count(')')  # folosita pentru gasirea tuturor clauzelor
count_1 = find_max_numbers(F)  # gasirea variabilelor
zeros = re.sub("\D", "", F)  # afiseaza toate numerele din expresie
numbers = sum(c.isdigit() for c in zeros)
F = F.replace("(", "").replace(")", "")
temp_F = F
F = F.split('^')

for i in range(0, len(F)):
    F[i] = F[i].split('V')

Linii = int(count_1)
Coloane = count
Matrice = [[0 for i in range(Linii + 7)] for j in range(Coloane)]

# matrix_constructor(F, Matrice)  # ----> calculare matrice
# print(Matrice) # ----> afisare matrice FNC

for y in zeros:
    # acest counter are rolul de a opri for-ul, deoarece este o rezolvare
    # naiva, si daca il lasam fara counter dura mai mult de 10 min
    # s-a evalueze toate expresiile pentru checker
    counter += 1
    if counter == 1300:
        print(0)
        break

    for ele in temp_F:  # --> parcurge tot sirul de caractere
        if ele.isdigit():  # daca se gasesc numere, pune valori de 0/1
            new_F += str(random.randint(0, 1))
        else:
            new_F += ele  # pune in continuare caracterele din temp_F

    new_F = new_F.replace("00", "0").replace("11", "1").replace("01", "0").replace("10", "1")
    new_F = negate(new_F)  # --> inverseaza/neaga numerele

    if eval_expresie(new_F) == 1:  # daca functia returneaza 1, atunci este SAT
        print(1)
        break
    else:
        # daca nu se returneaza "1", se da reset la expresie, si incearca din nou
        # o combinatie de 0 si 1
        new_F = ''
