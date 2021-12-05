#include <bits/stdc++.h>
using namespace std;

/*
   Parcurgere DFS a grafului pentru a determina numarul de noduri din fiecare
   componenta conexa a grafului dupa ce a fost izolat un anumit nod.
*/
void dfs(int curr_node, int *no_nodes, int *visited, vector<int> graph[],
         int excluded_node) {
  // Se incrementeaza numarul de noduri
  (*no_nodes)++;

  /*
     Pentru fiecare nod copil nevizitat deja al nodului curent care este
     diferit de nodul izolat se continua parcurgerea si se marcheaza nodul
     ca fiind vizitat.
  */
  for (auto &child : graph[curr_node]) {
    if (visited[child] == 0 && child != excluded_node) {
      visited[child] = 1;
      dfs(child, no_nodes, visited, graph, excluded_node);
    }
  }
}

/*
   Functie pentru a calcula numarul de conexiuni pierdute la izolarea unui nod
   intr-un graf conex.
*/
int max_disconnections(int n, vector<int> graph[], int *visited,
                       int excluded_node) {
  int no_nodes, no_connections = 0;

  /*
     Se parcurge dfs fiecare componenta conexa si se salveaza numarul de
     conxiuni din fiecare.
  */
  for (int i = 1; i <= n; ++i) {
    if (i != excluded_node && visited[i] == 0) {
      no_nodes = 0;
      visited[i] = 1;

      dfs(i, &no_nodes, visited, graph, excluded_node);

      no_connections += no_nodes * (no_nodes - 1);
    }
  }

  /*
     Numarul de conexiuni pierdute este numarul initial de conexiuni minus
     numarul curent.
  */
  return n * (n - 1) - no_connections;
}

int main() {
  int N, M, x, y, lost_connections;
  FILE *in = fopen("retea.in", "rt");

  // Se citesc numarul de noduri si muchii.
  fscanf(in, "%d %d\n", &N, &M);

  vector<int> graph[N + 1];

  // Se citesc muchiile.
  for (int i = 0; i < M; ++i) {
    fscanf(in, "%d %d\n", &x, &y);

    graph[x].push_back(y);
    graph[y].push_back(x);
  }

  fclose(in);

  int visited[N + 1];

  FILE *out = fopen("retea.out", "wt");

  /*
     Se izoleaza fiecare nod pe rand si se afiseaza numarul de conexiuni
     pierdute.
  */
  for (int i = 1; i <= N; ++i) {
    memset(visited, 0, sizeof(visited));
    lost_connections = max_disconnections(N, graph, visited, i);

    fprintf(out, "%d\n", lost_connections);
  }

  fclose(out);

  return 0;
}
