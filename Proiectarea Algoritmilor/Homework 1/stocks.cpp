#include <bits/stdc++.h>

using namespace std;

  /* 
    Functie pentru calcularea profitul maxim pe care il poate obtine Gigel.
    Este de fapt problema rucsacului cu doua constrangeri in loc de una.
    Am pornit de la algoritmul din laborator.
  */
int getMaxWin(int currentValue[], int minValue[], int maxValue[], int N, int B,
                int L) {
    int i, j, k;

    /* 
      Se aloca memorie pentru matrice si se initializeaza cu 0 elementele
      acesteia. Deci nu mai este nevoie sa scriem cazul de baza.
    */
    int ***dp = (int ***)calloc((N + 1), sizeof(int **));

    for (i = 0; i <= N; ++i) {
      dp[i] = (int **)calloc((B + 1), sizeof(int *));
      for (j = 0; j <= B; ++j) {
        dp[i][j] = (int *)calloc((L + 1), sizeof(int));
      }
    }

    // Cazul general.
    for (i = 1; i <= N; ++i) {
      for (j = 0; j <= B; ++j) {
        for (k = 0; k <= L; ++k) {
          // Gigel nu cumpara actiunea i => e solutia de la pasul i - 1.
          dp[i][j][k] = dp[i - 1][j][k];

          /* 
            Gigel vrea sa cumpare actiunea i deci trebuie verificat daca se 
            incadreaza in bugetul sau si daca nu depaseste pierderea maxima 
            pe care si-o poate permite

            Adica atunci cand am selectat dintre primele i − 1 actiuni, nu 
            trebuia sa consum mai mult de (j - currentValue[i]) din buget si sa 
            nu am o potentiala pierdere maxima mai mare decat (k - minValue[i]).
          */
          if (j - currentValue[i] >= 0 && k - minValue[i] >= 0) {
            /* 
                Fata de subproblema mentionata, castig in plus maxValue[i] 
                (profitul pe care il poate aduce aceasta actiune).
            */
            int sol_aux =
                dp[i - 1][j - currentValue[i]][k - minValue[i]] + maxValue[i];

            dp[i][j][k] = max(dp[i - 1][j][k], sol_aux);
          }
        }
      }
    }

    /*
    Solutia problemei se gaseste in dp[N][B][L] (profitul maxim cumparand doar o 
    parte din actiunile disponibile).
    */
    return dp[N][B][L];
  }

int main() {
    int N, B, L, i;

    // Deschide fisierul de intrare.
    FILE *in = fopen("stocks.in", "rt");
    if (!in) {
    return 1;
    }

    /* 
      Citeste numarul de actiuni (N), bugetul lui Gigel (B) si pierderea maxima 
      pe care si-o permite (L).
    */
    fscanf(in, "%d %d %d", &N, &B, &L);

    // Pretul curent al fiecarei actiuni.
    int currentValue[N + 1];

    // Pretul minim al fiecarei actiuni.
    int minValue[N + 1];

    // Pretul maxim al fiecarei actiuni.
    int maxValue[N + 1];

    /* 
      Se citesc pretul curent, pretul minim si pretul maxim pentru fiecare 
      actiune si se calculeaza diferentele dintre pretul curent si cel minim,
      respectiv dintre cel maxim si cel curent pentru a determina 
      pierderea/catigul efectiv.
    */ 
    for (i = 1; i < N + 1; ++i) {
      fscanf(in, "%d %d %d", &currentValue[i], &minValue[i], &maxValue[i]);
      minValue[i] = currentValue[i] - minValue[i];
      maxValue[i] -= currentValue[i];
    }


    // Se calculeaza si se afiseaza profitul maxim pe care il poate obtine Gigel.
    int maxWin = getMaxWin(currentValue, minValue, maxValue, N, B, L);

    FILE *out = fopen("stocks.out", "wt");
    if (!out) {
      return 1;
    }

    fprintf(out, "%d", maxWin);
    fclose(out);

    return 0;
  }
