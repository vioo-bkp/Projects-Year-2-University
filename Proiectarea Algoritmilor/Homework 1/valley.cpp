#include <bits/stdc++.h>
using namespace std;

/* Functie care transforma o secventa de munti intr-o secventa de munti cu
inaltimi descrescatoare de la stanga la dreapta:
V i : valley[i - 1] >= valley[i]
*/
unsigned long long int makeDescedingPrefix(unsigned int *valley, int commonPosition) {
  int i;

  // Timpul necesar pentru a realiza transformarea completa.
  unsigned long long int time = 0;

  // Timpul necesar pentru a excava un munte.
  unsigned long long int currentSubtractionTime;

  for (i = 1; i <= commonPosition; ++i) {
    /* 
      Daca nu este respectata conditia trebuie excavat muntele [i] astfel 
      incat valley[i] == valley[i - 1] (facand astfel un numar minim de 
      sapaturi).

      Timpul de excavatie este egal cu numarul de sapaturi efectuate.
    */ 
    if (valley[i] > valley[i - 1]) {
      currentSubtractionTime = valley[i] - valley[i - 1];
      valley[i] -= currentSubtractionTime;
      time += currentSubtractionTime;
    }
  }

  // Se intoarce timpul necesar.
  return time;
}

/* Functie care transforma o secventa de munti intr-o secventa de munti cu
  inaltimi crescataore de la stanga la dreapta:
  V i : valley[i - 1] <= valley[i]
*/
unsigned long long int
makeAscendingSuffix(unsigned int *valley, int commonPosition, int endOfValley) {
  int i;

  // Timpul necesar pentru a realiza transformarea completa.
  unsigned long long int time = 0;

  // Timpul necesar pentru a excava un munte.
  unsigned long long int currentSubtractionTime;

  for (i = endOfValley - 1; i > commonPosition; --i) {
    /* 
      Daca nu este respectata conditia trebuie excavat muntele [i - 1] 
      astfel incat valley[i] == valley[i - 1] (facand astfel un numar minim 
      de sapaturi).

      Timpul de excavatie este egal cu numarul de sapaturi efectuate.
    */ 
    if (valley[i] < valley[i - 1]) {
      currentSubtractionTime = valley[i - 1] - valley[i];
      valley[i - 1] -= currentSubtractionTime;
      time += currentSubtractionTime;
    }
  }

  // Se intoarce timpul necesar.
  return time;
}

int main() {
  int N;

  // Deschide fisierul de intrare.
  FILE *in = fopen("valley.in", "rt");
  if (!in) {
    return 1;
  }

  // Citeste numarul de munti (N).
  fscanf(in, "%d", &N);

  // Aloca memorie pentru actualii munti/viitoarea vale.
  unsigned int *valley = (unsigned int *)calloc(N, sizeof(unsigned int));

  /*
    Citesc inaltimile primilor 2 munti.
    
    Avand in vedere constrangerile din enunt:
        - prefixul si sufixul ocupa cel putin doua pozitii;
        - prefixul si sufixul au o pozitie in comun.
    
    Este necesar sa stabilim acest punct comun ignorand primul, respectiv
    ultimul munte pentru a satisafce prima conditie.

    Punctul comun va fi setat initial ca al doilea munte din vector, iar
    ulterior va fi actualizat cu inaltimea minima a muntilor de dupa acesta,
    mai putin ultimul munte.
  */
  fscanf(in, "%u %u", &valley[0], &valley[1]);
  unsigned int commonPosition = 1;

  // Se citesc inaltimile celorlalti munti si se stabileste pozitia comuna.
  for (int i = 2; i < N; ++i) {
    fscanf(in, "%u", &valley[i]);

    if (valley[i] < valley[commonPosition] && i != N - 1) {
      commonPosition = i;
    }
  }

  fclose(in);

  unsigned long long int totalTime;

  /* Se calculeaza timpul necesar transformarii primei parti a vectorului 
    intr-o secventa de inaltimi descrescatoare.
  */  
  totalTime = makeDescedingPrefix(valley, commonPosition);

  /* La aceasta se adauga timpul necesar pentru transfornarea celei de-a 
    doua parti a vectorului intr-o secventa crescatoare.
  */
  totalTime += makeAscendingSuffix(valley, commonPosition, N);

  // Se afiseaza timpul total necesar.
  FILE *out = fopen("valley.out", "wt");
  if (!out) {
    return 1;
  }

  fprintf(out, "%llu", totalTime);
  fclose(out);

  return 0;
}
