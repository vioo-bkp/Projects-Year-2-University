#include <cstdio>
#include <vector>
#include <cassert>
#include <algorithm>
#define MAXLG 20
#define DOUBLE(N) ((N << 1) + 10)

struct edge {
    int x, y, c;
    int index;

    edge() {}

    edge(const int& _x, const int& _y, const int& _c, const int& _index)
        : x(_x), y(_y), c(_c), index(_index) {}

    /* compara dupa cost */
    bool operator<(const edge& e) const {
        return c < e.c;
    }
};


class Homework {
    private:
        /* N = numar de noduri
          M = numar de muchii
          Q = numar de queryuri */
        int N, M, Q;

        /* E[ i ] muchia cu indexul i */
        std::vector< edge > E;

        /* generarea APM stocare
            P[i] = parintele (parent) nodului i
                in multimea de paduri disjuncte -
                tatal va fi radacina respectivei componente conexe */
        int *P;
        /* apm este vectorul care contine DOAR muchiile din APM */
        std::vector< edge > apm;
        /* Costul APM-ului */
        long long cost;
        /* APM-ul (graf) reprezentat prin liste de adicenta */
        std::vector<int> *G;


        /* Variabile  utilizare pentru  LCA */
        /* viz[i] = true daca si numai daca nodul i a fost vizitat in DFS */
        bool *viz;
        /* Euler[i] = elementul de pe pozitia i din parcugerea Euler a grafului
           First[i] = primul pozitie(index) la care nodul i apare in parcurgere
                     (in vectorul Euler)
           logOf[i] = partea intreaga a lui logaritm in baza 2 din i
           Level[i] = nivelul nodului i din parcurgerea DFS
           RMQ[k][i] = nodul de nivel minim din vectorul Euler, pozitiile considerate
                        sunt [i, i + 2^k)
        */
        int *Euler, *First, *logOf, *RMQ[MAXLG], *Level;

        /* alocare variabile pentru stramosi
            s[k][i] = strmosul de ordin 2^k a lui i
            maxEdge[k][i] = costul maxim al unei muchii de pe lantul de la i la
                            stramosul de ordin 2^k al lui i
            lg = logaritm din N, variabila folosita pentru a determina care este
                cel mai mare k a.i. stramosul de ordin 2^k al unui nod din graful dat
                sa existe
        */
        int *s[MAXLG], *maxEdge[MAXLG], lg;

    public:
        Homework() {
            /* Initializare variabile */
            G = NULL;
            P = Euler = First = logOf = Level = NULL;

            for (int i = 0; i < MAXLG; ++i) {
                RMQ[i] = s[i] = maxEdge[i] = NULL;
            }
        }

        /* sterge toate tablorile alocate */
        ~Homework() {
            if (P) {
                delete[] P;
            }

            if (G) {
                delete[] G;
            }

            if (viz) {
                delete[] viz;
            }

            if (Euler) {
                delete[] Euler;
            }

            if (First) {
                delete[] First;
            }

            if (logOf) {
                delete[] logOf;
            }

            if (Level) {
                delete[] Level;
            }

            for (int  i = 0; i < MAXLG; ++i) {
                if (RMQ[i]) {
                    delete[] RMQ[i];
                }

                if (s[i]) {
                    delete[] s[i];
                }

                if (maxEdge) {
                    delete[] maxEdge[i];
                }
            }
        }

        /* ruleaza problema */
        void run() {
            /* citeste N si M */
            assert( scanf("%d%d%d", &N, &M, &Q) == 3 );

            /* citeste muchii */
            E.reserve( M );
            for (int muchie = 0; muchie < M; ++muchie) {
                int x, y, c;
                assert( scanf("%d%d%d", &x, &y, &c) == 3 );

                /* adauga muchia in graf */
                E.push_back( edge(x, y, c, muchie) );
            }

            /* calculeaza APM-ul */
            cost = Kruskal(E);


            /* acum voi lucra pe graful dat de apm, N noduri, N-1 muchii */
            G = new std::vector<int> [ N + 1 ]();
            for (auto e = apm.begin(); e != apm.end(); ++e) {
                int x = e->x, y = e->y;

                /* Adauga muchia x-y in graf*/
                G[ x ].push_back( y );
                G[ y ].push_back( x );
            }

            /* preproceseaza elemente necesare pentru  LCA */
            preproccessLCA();

            /* initializare variabile stramosi */
            initStramosi();

            /* procesate query */
            printf("%lld\n", cost);
            while (Q--) {
                query();
            }
        }

        /* djSet(x) returneaza care este indexul multimii din are face parte
            * acest index este defapt dat de un nod, radacina componentei conexe
            din care face parte nodul x. */
        inline int djSet(const int& x) {
            /* Daca x nu este radacina, atunci urc pana la radacina */
            if(P[ x ] != x) {
                P[ x ] = djSet( P[ x ] );
            }
            return P[ x ];
        }

        /* Reuneste multimile din care fac parte x si y*/
        inline void reunion(const int& x, const int& y) {
            /* o radacina devine tatal celeilalte radacini */
            P[ djSet( x ) ] = djSet( y );
        }


        /*
            Aplica algoritmul Kruskal pentru a determina APM
            ** atentie, E fara referinta pentru ca nu vreau sa modic vectorul E */
        long long Kruskal(std::vector<edge> E) {
            /* Costul APM-ului */
            long long cost = 0;

            /* sorteaza muchiile crescator dupa cost */
            std::sort(E.begin(), E.end());

            /* Aloca vectorul de tati. Initial am N multimi disjuncte */
            P  = new int[ N + 1 ];
            for (int i = 1; i <= N; ++i) {
                P[ i ] = i;
            }

            apm.reserve( N - 1 );
            /* Parcurge fiecare muchie */
            for (auto e = E.begin(); e != E.end(); ++e) {
                /* daca capetele muchiei sunt in multimi disjuncte */
                if (djSet( e->x ) != djSet( e->y )) {
                    /* reuneste multimile */
                    cost += e->c;
                    apm.push_back( *e );
                    reunion( e->x, e->y );

                    if (apm.size() == N - 1) {
                        break;
                    }
                }
            }

            return cost;
        }

        /* Parcurgere in adancime - Parcugere Euler */
        inline void DFS(const int& node, const int& level) {
            viz[node] = 1; /* marcheaza nodul ca fiind vizitat */
            Euler[ ++Euler[0] ] = node; /* Adauga-l la parcurgere */
            First[ node ] = Euler[ 0 ]; /* E prima aparitie in parcurgere */
            Level[ node ] = level; /* Salveaza nivelul */

            /* Parcurge vecinii */
            for(auto son = G[ node ].begin(); son != G[ node ].end(); ++son) {
                /* vecin nevizitat */
                if(!viz[ *son ]) {
                    DFS( *son, level + 1 );
                    /* Adauga nodul inca o data in parcurgere dupa fiecare fiu */
                    Euler[ ++Euler[0] ] = node;
                }
            }
        }

        /* Preproceseaza elementele necesare pentru calculul LCA */
        void preproccessLCA() {
            /* alocare tablouri */
            First = new int[ N + 1 ]();
            logOf = new int[ DOUBLE(N) ]();
            Level = new int[ N + 1 ]();
            Euler = new int[ DOUBLE(N) ]();
            viz = new bool[ N + 1 ]();
            for (int i = 0; i < MAXLG; ++i) {
                RMQ[ i ] = new int[ DOUBLE(N) ]();
            }

            /* Fa parcurgerea Euler */
            Euler[ 0 ] = 0;
            DFS(1, 1);

            /* Precalculeaza logaritmii */
            for(int i = 2; i <= Euler[0]; ++i) {
                logOf[i] = logOf[i >> 1] + 1;
            }

            /* Precalculeaza RMQ */
            for(int i = 1; i <= Euler[0]; ++i) {
                RMQ[ 0 ][ i ] = Euler[ i ];
            }

            int x, y;
            for(int k = 1; (1 << k) <= Euler[0]; ++k) {
                int maxi = Euler[0] - (1 << k);
                for(int i = 1; i < maxi ; ++i) {
                    /* Cel mai miceelement din intervalul [i, i + 2^k)
                        este minimmul dintre
                        * cel mai mic element din intervalul [i, i + 2^(k-1))
                        * cel mai mic element din intervalul [i + 2^(k-1), i + 2^k*/
                    x = RMQ[k - 1][ i ], y = RMQ[k - 1][i + (1 << (k-1) )];
                    if(Level[ x ] < Level[ y ]) {
                        RMQ[ k ][ i ]=x;
                    } else {
                        RMQ[ k ][ i ]=y;
                    }
                }
            }
        }

        /* Returneaza LCA-ul pentru perechea (x,y) */
        inline int getLCA(const int& x, const int& y) {
            /* Obtine range-ul determina de primele aparitii ale celor
                doua noduri in parcurgerea Euler */
            int left = First[ x ], right = First[ y ];
            if(left > right) {
                std::swap(left, right);
            }

            /* Sparge intervalul [left, right] in doua, alege minimul dintre solutii */
            int k = logOf[right - left + 1];
            int x1 = RMQ[ k ][ left ], x2 = RMQ[ k ][ right - (1<<k) + 1 ];

            /* Alege nodul de nivel minim */
            if(Level[ x1 ] < Level[ x2 ]){
                return x1;
            } else {
                return x2;
            }
        }

        /* Initializeaza dinamicile de tip stramos */
        void  initStramosi() {
            /* Initializeaza / aloca variabile */
            lg = log2(N);
            for (int i = 0; i <= lg; ++i) {
                s[ i ] = new int[ 2* N + 10 ]();
                maxEdge[ i ] = new int[ 2 * N + 10 ]();
            }

            /* Initializeaza lanturile de lungime 1 (muchiile) */
            for (auto e = apm.begin(); e != apm.end(); ++e) {
                int x = e->x, y = e->y, cost = e->c, node, son;

                if (Level[x] < Level[y]) {
                    node = x;
                    son = y;
                } else {
                    node = y;
                    son = x;
                }

                /* Stramosul de ordin 2^0 a lui node este son */
                s[ 0 ][ son ] = node;
                /* Costul maxim al unei muchii de pe lantul precizat este cost */
                maxEdge[ 0 ][ son ] = cost;
            }

            /* pentru toate valorile cu sens ale lui k
                (se cauta al 2^k-lea stramos)*/
            for (int k = 1; k <= lg; ++k) {
                for (int i = 1; i <= N; ++i) {
                    /* Stramosul de ordin 2^k a lui i, este stramosul de ordin
                       2^(k-1) al stramosului de ordin 2^(k-1) al lui i*/
                    s[ k ][ i ] = s[ k - 1 ][ s[ k - 1 ][ i ] ];
                    /* Costul maxim al unei muchii de pe lantul, este maximul dintre cele doua costuri */
                    maxEdge[ k ][ i ] = std::max(maxEdge[ k - 1 ][ i ], maxEdge[ k - 1 ][ s[ k - 1 ][ i ] ]);
                }
            }
        }

        /* Returneaza muchia de cost maxim de pe lantul care uneste pe node de un stramos */
        inline int getMaxEdgeUp(int node, const int& superparent) {
            /* p este ordinul stramosului */
            int p = Level[ node ] - Level[ superparent ];
            int sol = -1;

            /* descompune lantul mare in lanturi mai mici, folosind
            reprezentarea binara a lui p*/
            for (int k = 0; k <= lg && node != superparent; ++k, p >>= 1) {
                /* daca bituk k face parte din p */
                if (p & 1) {
                    sol = std::max( sol, maxEdge[ k ][ node ]);
                    node = s[ k ][ node ];
                }
            }

            return sol;
        }

        /* proceseaza un query din fisier - care e solutia de rezerva
         pentru o muchie data */
        inline void query() {
            long long e1, e2, sol, maxE;
            int index, x, y, c, lca;

            /* citeste indexul muchiei */
            assert( scanf("%d", &index) == 1 );
            --index;

            /* afla capetele muchiei, lcaul lor */
            x = E[ index ].x;
            y = E[ index ].y;
            c = E[ index ].c;
            lca = getLCA(x, y);

            /* calculeaza maximele de pe cele doua lanturi apoi
             de pe lantul mare */
            e1 = getMaxEdgeUp(x, lca);
            e2 = getMaxEdgeUp(y, lca);
            maxE = (e1 >= e2 ? e1 : e2 );

            /* costul minim se obtine eliminand muchia de cost maxim
            de la x la y si adaugand pe cea impusa */
            sol = cost - maxE + c;
            printf("%lld\n", sol);
        }
};

int main() {
    /* deschidere fisiere */
    FILE *fin = freopen("kim.in", "r", stdin);
    FILE *fout = freopen("kim.out", "w", stdout);

    Homework myAwesomeHomework;
    myAwesomeHomework.run();

    /* inchidere fisiere */
    fclose(fin);
    fclose(fout);

    return 0;
}
