#include <bits/stdc++.h>
using namespace std;

/*
   Structura pentru a retine o pozitie in matrice si distanta la care se afla
   fata de pozitia initiala.
*/
struct cell {
  pair<int, int> pos;
  int dist;
};

/*
   Functie pentru a verifica daca o pereche de coordonate se afla in afara
   matricei.
*/
bool check_pos(int x, int y, int M, int N) {
  return x >= 0 && x < M && y >= 0 && y < N;
}

/*
   Functie pentru a determina numarul minim de poduri pe care trebuie sa le
   traverseze Gigel pentru a ajunge pe uscat.

   Este de fapt vorba de algoritmul lui Lee care s-a studiat in liceu.
*/
int get_path(char **mat, int M, int N, pair<int, int> src) {
  int x, y;

  // Celula care nu contine niciun pod, Gigel se va ineca.
  if (mat[src.first][src.second] == '.') {
    return -1;
  }

  // Matrice pentru a retine in ce celule Gigel a fost deja.
  bool visited[M][N] = {false};
  visited[src.first][src.second] = true;

  // Celula de start din care porneste Gigel.
  cell source;
  source.pos = src;
  source.dist = 0;

  /*
     Se realizeaza o parcurgere BFS pentru a determina numarul minim de poduri
     pe care le va parcurge Gigel.
  */
  queue<cell> q;
  q.push(source);

  while (!q.empty()) {
    // Se extrage cate un element din coada.
    cell current = q.front();
    q.pop();

    /*
       Se extrag coordonatele la care s-ar afla Gigel si se verifica daca sunt
       valide.
    */
    pair<int, int> coords = current.pos;
    if (!check_pos(coords.first, coords.second, M, N)) {
      return current.dist;
    }

    /*
       Daca podul este unul Vertical sau Bidirectional se calculeaza noile
       coordonate la care ar putea ajunge Gigel.
    */
    if (mat[coords.first][coords.second] == 'V' ||
        mat[coords.first][coords.second] == 'D') {
      // Gigel se va deplasa pe linie.
      int d_x[] = {1, -1};

      // Se calculeaza noile coordonate.
      for (int i = 0; i < 2; i++) {
        x = coords.first + d_x[i];
        y = coords.second;

        // Daca Gigel a ajuns in afara hartii am gasit drumul dorit.
        if (!check_pos(x, y, M, N)) {
          return current.dist + 1;
        }

        /*
           Altfel se verifica daca Gigel nu a mai fost vreodata la acele
           coordonate si daca acestea duc catre un nou pod.
        */
        if (!visited[x][y] && mat[x][y] != '.') {
          visited[x][y] = true;

          /*
             Se creeaza o noua celula cu distanta si coordonate actualizate si
             se adauga in coada.
          */
          cell next;
          next.pos = make_pair(x, y);
          next.dist = current.dist + 1;

          q.push(next);
        }
      }
    }

    /*
       Daca podul este unul Orizontal sau Bidirectional se calculeaza noile
       coordonate la care ar putea ajunge Gigel.
    */
    if (mat[coords.first][coords.second] == 'O' ||
        mat[coords.first][coords.second] == 'D') {
      // Gigel se va deplasa pe coloana.
      int d_y[] = {1, -1};

      // Se calculeaza noile coordonate.
      for (int i = 0; i < 2; i++) {
        x = coords.first;
        y = coords.second + d_y[i];

        // Daca Gigel a ajuns in afara hartii am gasit drumul dorit.
        if (!check_pos(x, y, M, N)) {
          return current.dist + 1;
        }

        /*
           Altfel se verifica daca Gigel nu a mai fost vreodata la acele
           coordonate si daca acestea duc catre un nou pod.
        */
        if (!visited[x][y] && mat[x][y] != '.') {
          visited[x][y] = true;

          /*
             Se creeaza o noua celula cu distanta si coordonate actualizate si
             se adauga in coada.
          */
          cell next;
          next.pos = make_pair(x, y);
          next.dist = current.dist + 1;

          q.push(next);
        }
      }
    }
  }

  // Nu s-a gasit nicin drum pentru Gigel.
  return -1;
}

int main() {
  int N, M, x, y;
  FILE *in = fopen("poduri.in", "rt");

  // Se citesc numarul de linii si de coloane al matricei.
  fscanf(in, "%d %d\n", &M, &N);

  // Se citeste pozitia initiala a lui Gigel.
  fscanf(in, "%d %d\n", &x, &y);
  x--;
  y--;

  // Se aloca memorie pentru matrice si se citeste aceasta.
  char **mat = (char **)calloc(M, sizeof(char *));
  for (int i = 0; i < M; ++i) {
    mat[i] = (char *)calloc(N, sizeof(char));

    for (int j = 0; j < N; ++j) {
      fscanf(in, "%c", &mat[i][j]);
    }

    fscanf(in, "\n");
  }

  fclose(in);

  // Pozitia initiala la care se afla Gigel
  pair<int, int> source = std::make_pair(x, y);

  // Se calculeaza drumul minim.
  int distance = get_path(mat, M, N, source);

  FILE *out = fopen("poduri.out", "wt");
  fprintf(out, "%d\n", distance);
  fclose(out);

  return 0;
}
