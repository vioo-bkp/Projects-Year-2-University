# Tema 1 - AA - Turing Machine
# Strategy: Class and Function
# Autor: Mateescu Viorel - Cristian

import sys

class TuringMachine:
    # Initializarea unor componente care constituie masina Turing.
    def __init__(self):
        self.transitions = {}
        self.final_states = []
        self.no_states = 0

    # Construirea unei masini Turing pe baza unui input primit de la tastatura.
    def read_tm(self):
        # Se citeste numarul de stari al masinii.
        self.no_states = int(sys.stdin.readline())

        # Se citesc starile finale si se stocheaza in array daca exista macar una, altfel array-ul ramane gol.
        final_states = sys.stdin.readline().split()
        if final_states[0] != '-':
            self.final_states = final_states

        # Se citesc tranzitiile.
        for line in sys.stdin:
            cur_state, cur_symbol, new_state, new_symbol, tape_head_pos = line.split()

            # Se realizeaza codificarea trazitiilor in dictionar pe baza perechii (stare curenta, simbol curent).
            self.transitions[cur_state, cur_symbol] = tuple([new_state, new_symbol, tape_head_pos])

    # Simularea unui pas din executia masinii Turing.
    # Se primeste ca parametru o configuratie si se returneaza configuratia rezultata dupa executia unui pas al masinii
    # sau False in cazul in care masina se agata.
    # O configuratie este de tipul: (cuvant_stanga[0], stare_curenta[1], cuvant_dreapta[2]).
    def step(self, configuration):
        # Sunt extrase starea curenta si simbolul curent din configuratie.
        cur_symbol = configuration[2][0]
        cur_state = configuration[1]

        # Se incearca extragerea din dictionarul de tranzitii a tranzitiei corespunzatoare simbolului curent si a
        # starii curente in care se afla masina.
        action = self.transitions.get(tuple([cur_state, cur_symbol]))

        # Daca acesta exista se executa pasul si se returneaza noua configuratie, altfel se returneaza False.
        if action:
            # Se extrag simbolul si starea urmatoare.
            new_symbol = action[1]
            new_state = action[0]

            # Se suprascriu simbolul vechi si starea veche cu cele viitoare.
            new_configuration = configuration
            new_configuration[1] = new_state
            new_configuration[2] = new_configuration[2].replace(cur_symbol, new_symbol, 1)

            # Se realizeaza miscarea cursorului: dreapta(R), stanga(S) sau aceasta nu se modifica(H).
            # In cazul in care la stanga sau la dreapta cursorului nu ramane niciun caracter se adauga '#' in locul
            # respectiv.

            # La dreapta: se ia caracterul din dreapta cursorului si se muta in stanga acestuia.
            if action[2] == 'R':
                new_configuration[2] = new_configuration[2][1:]
                new_configuration[0] = new_configuration[0] + new_symbol

                if not new_configuration[2]:
                    new_configuration[2] = '#'

            # La stanga: se ia caracterul din stanga cursorului si se muta in dreapta acestuia.
            elif action[2] == 'L':
                new_configuration[2] = new_configuration[0][-1] + new_configuration[2]
                new_configuration[0] = new_configuration[0][:-1]

                if not new_configuration[0]:
                    new_configuration[0] = '#'

            return new_configuration

        return False

    # Se verifica daca un cuvant este acceptat de masina Turing(returneaza True) sau este respins de aceasta(returneaza
    # False).
    def accept(self, word):
        # Se seteaza configuratia initiala.
        configuration = ['#', '0', word]

        # Se simuleaza executia masinii Turing pe cuvantul respectiv pana cand aceasta ajunge intr-o stare
        # finala(accepta cuvantul) sau intr-o stare din care nu mai poate efectua alte tranzitii(respinge cuvantul).
        while configuration[1] not in self.final_states:
            configuration = self.step(configuration)
            if not configuration:
                return False

        return True

    # Se verifica daca un cuvant este acceptat de masina Turing in maximum steps(k) pasi(returneaza True), sau este
    # respins de aceasta(returneaza False).
    def k_accept(self, steps, word):
        # Se seteaza configuratia initiala.
        configuration = ['#', '0', word]

        # Se simuleaza executia masinii Turing pe cuvantul respectiv in maximum steps(k) pasi, astfel:
        # - masina accepta daca a ajuns intr-o stare finala in 1 sau mai multi pasi(maximum steps(k));
        # - masina respinge daca a ajuns intr-o stare nonfinala din care nu mai poate efectua alte tranzitii sau daca nu
        # a acceptat in maximum steps(k) pasi.
        for index in range(steps):
            configuration = self.step(configuration)
            if configuration is False:
                return False

            new_state = configuration[1]
            if new_state in self.final_states:
                return True

        return False


# Main-ul:
def main():
    # Se citeste tipul task-ului.
    cmd = sys.stdin.readline()
    cmd = cmd.strip('\n')

    # Se citeste input-ul pentru task-ul respectv si se separa pe componente.
    task_input = sys.stdin.readline().split()

    # Se creeaza si se citeste masina Turing.
    tm = TuringMachine()
    tm.read_tm()

    # Task step:
    if cmd == "step":
        # Se prelucreaza configuratiile, se simuleaza cate un pas din executia masinii Turing pentru fiecare si se
        # afiseaza rezultatul acestuia: noua configuratie sau False.
        for config in task_input:
            config = config.strip("()")

            cur_config = config.split(',')
            next_config = tm.step(cur_config)

            if next_config is False:
                print("False", end=' ')
            else:
                print('(', next_config[0], ',', next_config[1], ',', next_config[2], ')', end=' ', sep='')

    # Task accept:
    elif cmd == "accept":
        # Se verifica daca masina Turing accepta cuvintele respective si afiseaza True(accepta) sau False(respinge).
        print(*[tm.accept(words) for words in task_input], end=' ')

    # Task k_accept:
    elif cmd == "k_accept":
        # Se verifica daca masina Turing accepta cuvintele respective intr-un numar maxim de pasi si afiseaza
        # True(accepta) sau False(respinge).
        word_steps = [elem.split(',') for elem in task_input]
        print(*[tm.k_accept(int(steps), word) for word, steps in word_steps], end=' ')


# Apel main:
if __name__ == "__main__":
    main()