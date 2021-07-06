#include <bits/stdc++.h>
using namespace std;

int i = 0, j = 0, l = 0, temp_mat;

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    int v[m];
    int matrice[m][n];

    for (i = 0; i<m; i++) {
      for(j = 0;j<n;j++) {
         scanf("%d", &matrice[i][j]);
      }
   }

	for (i = 0; i<m; ++i) {
		for (j = 0; j<n; ++j) {
			for (l = j+1; l < n; ++l)
				if (matrice[i][j] > matrice[i][l]) {
					temp_mat = matrice[i][j];
					matrice[i][j] = matrice[i][l];
					matrice[i][l] = temp_mat;
				}
            }
	    }

    for (int i = 0; i <m; ++i){
        scanf("%d", &v[i]);
    }

    for (i = 0; i < m; ++i){
        printf("%d ", matrice[i] [v[i]]);
    }

   return 0;

}
