# Etape:
Mateescu Viorel-Cristian

- In general algoritmul BDD-SAT este mai rapid, mai eficient decat algoritmul CNF-SAT,
obtinand timpi de executie mai mici, mai buni decat cel din urma;
- Timpul de executie creste odata cu cresterea numarului de variabile din expresie. Pentru
un numar de variabile mai mic algoritmul CNF-SAT obtine timpi de executie mai mici sau
aproximativ egali cu cei ai algoritmului BDD-SAT (concluzie trasa pe baza primelor 5
teste);
- O diferenta majora din acest punct de vedere se remarca pentru expresiile cu un numar de
variabile mai mare si mai ales pentru cele nesatisfiabile, unde algoritmul BDD-SAT se
evidentiaza prin timpi de executie mult mai mici fata de algoritmul CNF-SAT (0.71s vs
34s, 0.36s vs 12s etc);
- Aceasta discrepanta este generata de faptul ca algoritmul CNF-SAT necesita constructia
unei interpretari complete pentru a determina daca aceasta satisface sau nu formula (se
verifica daca interpretarea satisaface fiecare clauza din formula);
- Insa algoritmul BDDSAT se opreste din constructia unei interpretari (constructia ramurii respective a arborelui)
in momentul in care o clauza din cadrul formulei nu este satisfacuta de aceasta, pe fiecare
nivel fiind inlocuit cate un literal din expresie cu valorile de adevar posibile (True/False)
si reevaluandu-se fiecare clauza din aceasta.