#include <cstdio>
#include <vector>
#include <cassert>

/* Functia returneaza true daca si numai daca in urma parcurgerii
DFS pornind din nod se constanta ca exista un ciclu in componenta
conexa curenta */
bool treeHasCycle(const int& nod, std::vector<int>*& G, int*& T);

int main() {
    /* deschidere fisiere */
    FILE *fin = freopen("portal.in", "r", stdin);
    FILE *fout = freopen("portal.out", "w", stdout);

    /* N = numar de noduri, M = numar de muchii */
    int N, M; 

    /* G[ nod ] = lista de adiacenta a nodului nod */
    std::vector<int> *G;

    /* T[ nod ] = tatal lui nod obtinut in urma parcurgerii DFS */
    int *T;

    /* citeste N si M */
    assert( scanf("%d%d", &N, &M) == 2 );

    /* Aloca trablouurile */
    T = new int [ N + 1 ];
    G = new std::vector<int> [ N + 1 ];

    /* citeste muchii */
    for (int muchie = 0; muchie < M; ++muchie) {
        int x, y;
        assert( scanf("%d%d", &x, &y) == 2 );

        /* adauga muchia in graf */
        G[x].push_back(y);
        G[y].push_back(x);
    }

    /* initializeaza solutia si vectorul de tati */
    int sol = 0; /* numal de noduri izolate */
    for (int nod = 1; nod <= N; ++nod) {
        T[ nod ] = 0;
    }

    /* parcurge componentea cu componenta si analizeaza */
    for (int nod = 1; nod <= N; ++nod) {
        /* daca nodul nod nu are tata nu a mai fost vizitat */
        if (!T[ nod ]) {
            /* el va fi radacina unui arbore DFS, tatal lui este el
               prin convetie (marchez faptul ca l-am vizitat) */
            T[ nod ] = nod; 

            /* Vad daca in aceasta componenta conexa se afla un ciclu
               Daca nu are un ciclu, atunci va ramane un nod izolat */
            if (!treeHasCycle( nod, G, T)) {
                ++sol;
            }
        }
    }

    /* afiseaza solutia */
    printf("%d\n", sol);

    /* elibereaza memoria */
    delete[] T;
    delete[] G;

    /* inchidere fisiere */
    fclose(fin);
    fclose(fout);

    return 0;
}


bool treeHasCycle(const int& nod, std::vector<int>*& G, int*& T) {
    /* presupun ca nu exista ciclu in subgraful
       care porneste din nod */
    bool cycle = false;

    /* parcurg vecinii */
    for (auto it : G[nod]) {
        /* daca nu are tata, il vizitez acum */
        if (!T[ it ]) {
            T[ it ] = nod; /* acum are tata */
            cycle = cycle || treeHasCycle( it, G, T );
        } else {
            /* are tata, deci a fost vizitat
               daca it nu este tatal lui nod,
               atunci (nod, it) este muchie de intoarcere */
            if (it != T[nod]) {
                /* am gasit ciclul */
                cycle = 1; 
            }
        }
    }

    /* returneaza raspunsul la intreabarea exista ciclu
      in componenta conexa ? */
    return cycle;
}