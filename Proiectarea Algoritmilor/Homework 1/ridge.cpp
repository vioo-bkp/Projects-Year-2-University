#include <bits/stdc++.h>

using namespace std;

/* Functie pentru calculul costului minim de excavatie pentru a obtine o 
   creasta montana dintr-un vector de munti.

   Creasta montana = nu exista doi munti adiacenti de aceeasi inaltime.

   Pentru a indeplini aceasta conditie Gigel trebuie sa faca 0, 1 sau 2 
   sapataturi in fiecare munte.
*/
unsigned long long int minimumCost(int mountains[], int costs[], int N) {
  int i, j, k;

  /* 
     Costul minim pentru a obtine o creasta montana.
     
     Initial este setat pe cea mai mare valoare posibila pe unsigned long long 
     int (-1 din reprezentarea signed si se face conversie automat la fara 
     semn) deoarece acesta va fi actualizat folosind functia min.
  */
  unsigned long long int minCost = -1;

  // Costul minim de la pasul anterior.
  unsigned long long int minPrevCost;

  /* 
     Se aloca memorie pentru matrice si se initializeaza cu 0 elementele
     acesteia. Deci nu mai este nevoie sa scriem cazul de baza dp[0][0] = 0.
  */
  unsigned long long int **dp =
      (unsigned long long int **)calloc(N, sizeof(unsigned long long int *));
  for (i = 0; i < N; ++i) {
    dp[i] = (unsigned long long int *)calloc(3, sizeof(unsigned long long int));
  }

  // Cazuri de baza.

  // Se sapa o singura data in primul munte.
  dp[0][1] = costs[0];

  // Se sapa de doua ori in primul munte.
  dp[0][2] = costs[0] * 2;

  // Cazul general.
  for (i = 1; i < N; ++i) {
    for (j = 0; j < 3; ++j) {
      /* 
         Nu se pot face j excavatii in muntele i => setam dp[i][j] la cea mai 
         mare valoare posibila.
      */
      dp[i][j] = -1;

      // Verfica daca se pot face j excavatii in muntele i.
      if (mountains[i] - j >= 0) {
        /* 
           Seteaza costul de excavatie la pasul anterior la cea mai mare 
           valoare posibila, el fiind actualizat ulterior cu functia min.
        */
        minPrevCost = -1;

        /*
           Se calculeaza costul minim de excavatie la pasul anterior.
        */
        for (k = 0; k < 3; ++k) {
          /* 
             Se verifica daca prin excavarea a j unitati in muntele i se
             obtine o valoarea diferita de cea obtina la pasul anterior prin 
             excavarea a k unitati in muntele (i - 1) si daca exacavarea a k
             unitati este posibila in muntele (i - 1).
          */
          if (mountains[i] - j != mountains[i - 1] - k &&
              mountains[i - 1] - k >= 0) {
            /* 
               Actualizeaza costul minim de excavatie la pasul anterior cu 
               valoarea minima obtinuta in cazul excavarilor posibile.
            */
            minPrevCost = min(minPrevCost, dp[i - 1][k]);
          }
        }

        /* 
           Se calculeaza costul minim la pasul i pentru j excavatii in 
           muntele i.
        */
        dp[i][j] = j * costs[i] + minPrevCost;
      }
    }
  }

  // Costul minim este minimul dintre cele 3 costuri obtinute pe linia N - 1.
  for (int i = 0; i < 3; ++i) {
    minCost = min(minCost, dp[N - 1][i]);
  }

  return minCost;
}

int main() {
  int N;

  // Deschide fisierul de intrare.
  FILE *in = fopen("ridge.in", "rt");

  // Citeste numarul de munti (N).
  fscanf(in, "%d", &N);

  // Inaltimea fiecarui munte.
  int mountains[N];

  // Costul de excavatie in fiecare munte.
  int costs[N];

  // Se citesc inaltimea si costul de excavatie pentru fiecare munte.
  for (int i = 0; i < N; ++i) {
    fscanf(in, "%d %d", &mountains[i], &costs[i]);
  }

  fclose(in);

  /* 
     Se calculeaza si se afiseaza costul minim de excavatie pentru a obtine o 
     creasta montana.
  */
  unsigned long long int minCost = minimumCost(mountains, costs, N);

  FILE *out = fopen("ridge.out", "wt");
  if (!out) {
    return 1;
  }

  fprintf(out, "%llu", minCost);
  fclose(out);

  return 0;
}
