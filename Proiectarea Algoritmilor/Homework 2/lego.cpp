#include <bits/stdc++.h>
using namespace std;

/*
   Functie de backtracking pentru a vedea ce piese putem construi cu setul
   de dimensiuni primit ca parametru.
*/
void build_max_dimensions(int new_dimension, bool *dimensions,
                          int max_dimension, int max_pieces, list<int> sizes,
                          int no_used_pieces) {
  /*
     S-a depasit dimensiunea maxima ce poate fi construita cu ajutorul setului
     de piese.
  */
  if (new_dimension > max_dimension) {
    return;
  }

  // Se salveaza faptul ca dimensiunea este realizabila.
  dimensions[new_dimension] = true;

  // Daca s-a atins numarul maxim de piese nu mai putem genera dimensiuni.
  if (no_used_pieces == max_pieces) {
    return;
  }

  // Se genereaza toate dimensiunile posibile folosind backtracking.
  for (auto aux_dimension : sizes) {
    /*
       Daca nu s-a atins numarul maxim de piese admise pentru o dimensiune
       si dimensiunea este mai mica sau egala cu dimensiunea maxima admisa
       se genereaza dimensiuni in continuarea acesteia folosind piesele
       disponibile.
    */
    if (no_used_pieces + 1 <= max_pieces &&
        new_dimension + aux_dimension <= max_dimension) {
      build_max_dimensions(new_dimension + aux_dimension, dimensions,
                           max_dimension, max_pieces, sizes,
                           no_used_pieces + 1);
    }
  }
}

int check_solution(int max_dimension, list<int> sizes, int max_pieces) {
  // Vector pentru a retine dimensiunile realizabile cu setul de piese.
  bool dimensions[max_dimension + 1] = {false};

  // Se genereaza dimensiunile.
  build_max_dimensions(0, dimensions, max_dimension, max_pieces, sizes, 0);

  // Se determina sirul de dimensiuni consecutive de lungime maxima.
  int curr_dimensions = 0, max_dimension_built = 0;
  for (int i = 1; i <= max_dimension; ++i) {
    if (dimensions[i]) {
      curr_dimensions++;
    } else {
      if (curr_dimensions > max_dimension_built) {
        max_dimension_built = curr_dimensions;
      }
      curr_dimensions = 0;
    }
  }

  if (curr_dimensions > max_dimension_built) {
    max_dimension_built = curr_dimensions;
  }

  // Se returneaza lungimea maxima obtinuta.
  return max_dimension_built;
}

/*
   Functie pentru generarea a n combinari luate cate k.

   Am scris la laborator si am adaptat-o aici.
*/
void back(list<int> base_dimensions, list<int> solution,
          unsigned int no_used_dimensions, int max_use_of_piece,
          list<int> *dimensions_built, int *max_dimension_built) {
  /*
     Daca s-a atins numarul dorit de piese de baza se calcleaza dimensiunile
     ce pot fi obtinute cu acestea.
  */
  if (solution.size() == no_used_dimensions) {
    int current_max_dimension = check_solution(
        max_use_of_piece * solution.back(), solution, max_use_of_piece);

    /*
       Daca s-a obtinut un sir de dimensiuni consecutive de lungime mai mare
       decat anteriorul se updateaza.
    */
    if (current_max_dimension > *max_dimension_built) {
      *max_dimension_built = current_max_dimension;
      *dimensions_built = solution;
    }

    return;
  }

  // Lista din care vom elimina fiecare dimensiune utilizata la pasul curent.
  list<int> aux = base_dimensions;

  // Se genereaza combinari.
  while (!aux.empty()) {
    // Combinarile trebuie sa inceapa cu 1 (conditie din enunt).
    if (solution.empty() && aux.front() != 1) {
      break;
    }

    // Se extrage valoarea si se adauga la solutie.
    int value = aux.front();
    solution.push_back(value);
    aux.remove(value);

    // Stergem din dimensiunile disponibile dimensiunea utilizata.
    base_dimensions.remove(value);

    // Generam restul solutiei.
    back(base_dimensions, solution, no_used_dimensions, max_use_of_piece,
         dimensions_built, max_dimension_built);

    // Scoatem din solutie valoarea de la pasul curent.
    solution.remove(value);
  }
}

int main() {
  int N, T, K;
  FILE *in = fopen("lego.in", "rt");

  /*
     Se citesc dimensiunea maxima, numarul de piese selectate si numarul maxim
     de piese ce pot fi utilizate.
  */
  fscanf(in, "%d %d %d\n", &K, &N, &T);

  fclose(in);

  int *dimensions = (int *)calloc(K, sizeof(int));
  list<int> sol;

  // Dimensiunile de baza
  for (int i = 0; i < K; ++i) {
    dimensions[i] = i + 1;
  }

  std::list<int> dimension_list(dimensions, dimensions + K);

  /*
     Numarul maxim de dimensiuni consecutive ce pot obtinute si lista
     pieselor de baza utilizat pentru acestea.
  */
  int max_dimensions = 0;
  list<int> max_dimensions_list;

  // Generarea solutiei.
  back(dimension_list, sol, N, T, &max_dimensions_list, &max_dimensions);

  FILE *out = fopen("lego.out", "wt");

  /*
     Se afiseaza mumarul maxim de dimensiuni consecutive ce pot obtinute si
     lista pieselor de baza utilizat pentru acestea.
  */
  fprintf(out, "%d\n", max_dimensions);

  for (auto dim : max_dimensions_list) {
    fprintf(out, "%d ", dim);
  }

  fclose(out);

  return 0;
}
