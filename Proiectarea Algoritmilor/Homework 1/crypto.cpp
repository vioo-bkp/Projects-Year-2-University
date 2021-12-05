#include <bits/stdc++.h>

using namespace std;

/* 
   Functie pentru calcularea numarului maxim de monede pe care calculatoarele 
   lui Gigel le-ar putea produce intr-o ora.
*/
int getMaxCrypto(std::pair<int, int> PCs[], int N, int B) {
  // Suma costurilor de upgrade ale PC-urilor la pasii anteriori.
  int partialCost = 0;

  // Costul de upgrade al calculatoarelor la pasul curent.
  int currentUpgradesCost = 0;

  // Numarul curent de monede pe care reteaua lui Gigel o poate produce.
  int currentNoCryptos = PCs[0].first;

  int i = 0;

  // Cat timp nu a fost depasit bugetul lui Gigel:
  while (partialCost + currentUpgradesCost <= B) {
  	// Se actualizeza costul partial.
    partialCost += currentUpgradesCost;

    /* 
       Se va upgrada cate un calculator pe rand pana cand toate vor produce
       acelasi numar de moneda si le vom putea upgrada pe toate simultan. 
    */
    for (; PCs[i].first == currentNoCryptos && i < N; ++i) {
      PCs[i].first += 1;
      currentUpgradesCost += PCs[i].second;

      // Se verifica daca s-a depasit bugetul lui Gigel.
      if (partialCost + currentUpgradesCost > B) {
        return currentNoCryptos;
      }
    }

    // Se incrementeaza numarul de monede pe care reteaua il poate produce.
    currentNoCryptos += 1;
  }

  return currentNoCryptos - 1;
}

int main() {
  int N, B, i, coins, upgradeCost;

  // Deschide fisierul de intrare.
  FILE *in = fopen("crypto.in", "rt");
  if (!in) {
    return 1;
  }

  // Se citesc numarul de calculatoare (N) si bugetul lui Gigel (B).
  fscanf(in, "%d %d", &N, &B);

  /* 
  	 Vector de calculatore care are ca date perechi formate din:
     	- numarul maxim de monede produse de calculator pe ora;
     	- costul de upgrade al calculatorului.
  */
  std::pair<int, int> PCs[N];

  // Se citesc informatiile despre calculatoare.
  for (i = 0; i < N; ++i) {
    fscanf(in, "%d %d", &coins, &upgradeCost);
    PCs[i] = std::make_pair(coins, upgradeCost);
  }

  fclose(in);

  /* 
     Se sorteaza calcultoarele crescator dupa numarul maxim de monede produse 
     pe ora de acestea.
  */
  sort(PCs, PCs + N);

  /* 
     Se calculeaza si se afiseaza numarul maxim de monede pe care 
     calculatoarele lui Gigel le-ar putea produce intr-o ora.
  */
  int maxNoCryptos = getMaxCrypto(PCs, N, B);

  FILE *out = fopen("crypto.out", "wt");
  if (!out) {
    return 1;
  }

  fprintf(out, "%d", maxNoCryptos);
  fclose(out);

  return 0;
}
