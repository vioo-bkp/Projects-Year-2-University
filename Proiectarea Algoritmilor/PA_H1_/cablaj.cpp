#include <cstdio>
#include <cstring>
#include <algorithm>
#define NMAX 109
#define oo (1 << 20) /* Compute infinit value */


/* build matrix with dynammic programming */
void makeDP(char *s, const int& n, int **D, int **H);
/* rebuild the solution */
void rebuild(const int& n, int **D, int **H, int i, int j);

int main() {
	/* open the files */
	freopen("cablaj.in", "r", stdin);
	freopen("cablaj.out", "w", stdout);

	/* input string */
	char s[NMAX];
	/* D[i][j] = best solution for [i,j] */
	int **D;
	/* H[i][j] = the height corresponding for [i,j]
				 when I've computed best solution D[i][j] */	
	int **H;
	/* size of string */
	int n;

	/* read input */
	scanf("%d%s", &n, (s+1) );
	n = 2*n;

	/* Allocate matrix */
	D = new int* [n + 1];
	H = new int* [n + 1];
	for (int i = 0; i <= n; ++i) {
		D[i] = new int[n + 1];
		H[i] = new int[n + 1];
	}

	/* Build D and H */
	makeDP(s, n, D, H);

	/* print cost */
	printf("%d\n", D[1][n]);
	/* recursively rebuild the solution */
	rebuild(n, D, H, 1, n);

	/* Deallocate matrix */
	for (int i = 0; i <= n; ++i) {
		delete[] D[i];
		delete[] H[i];
	}
	delete[] D;
	delete[] H;

	return 0;
}

/* build matrix with dynammic programming 
	s =  input string
	n =  strlen(s)
	D[i][j] = best solution for [i,j] 
	H[i][j] = the height corresponding for [i,j]
				 when I've computed best solution D[i][j]	*/
void makeDP(char *s, const int& n, int **D, int **H) {
	int height, cost;

	/* initialization */
	for (int i = 1; i <= n; ++i) {
		for (int j = i; j <= n; ++j) {
			/* i doesn't match with j*/
			D[i][j] = H[i][j] = oo;
		}

		/* Check if i matches i + 1 */
		if (i < n && s[i] != s[i + 1]) {
			D[i][i + 1] = 3;
			H[i][i + 1] = 1;
		}
	}

	/* Compute matrix traversing diagonals */
	for (int d = 3; d <= n; d += 2) { /* d =  current diagonal */
		for (int i = 1; i < n; ++i) { /* i = current line */
			int j = i + d; /* current coulumn */

			if (j > n) {
				continue;
			}
			/* Compute D[i][j], H[i][j] */

			/* Check if s[i] matches s[j] */
			if (s[i] != s[j]) {
				height = 1 + H[i + 1][j - 1];
				cost = j - i + 2 * height + D[i + 1][j - 1];

				/* Check if this solution is better */
				if (cost < D[i][j]) {
					D[i][j] = cost;
					H[i][j] = height;
				} else {
					/* Or this solution is as good as D[i][j] but
					I can get a smaller height */
					if (cost == D[i][j] && height < H[i][j]) {
						H[i][j] = height;
					}
				}
			}

			/* I'll split in [i,k] [k+1,j] */
			for (int k = i + 1; k < j; k += 2) {
				cost = D[i][k] + D[k + 1][j];
				height = std::max(H[i][k], H[k + 1][j]);

				/* Check if this solution is better */
				if (cost < D[i][j]) {
					D[i][j] = cost;
					H[i][j] = height;
				} else {
					/* Or this solution is as good as D[i][j] but
					I can get a smaller height */
					if (cost == D[i][j] && height < H[i][j]) {
						H[i][j] = height;
					}
				}
			}
		}
	}
}

/* rebuild the solution - split [i,j] until finish */
void rebuild(const int& n, int **D, int **H, int i, int j) {
	int height, cost;

	/* Check if I have something to rebuild */
	if (i > j) {
		return;
	}

	/* Case when s[i] matches s[j] */
	height = 1 + H[i + 1][j - 1];
	cost = j - i + 2 * height + D[i + 1][j - 1];
	if (cost == D[i][j] && height == H[i][j]) {
		printf("%d %d\n", i, j); /* print current pair */
		rebuild(n, D, H, i + 1, j - 1); /* Rebuild remaining path */
		return;
	}

	/* Case when I've split in [i,k] [k+1,j] */
	for (int k = i + 1; k < j; k += 2) {
		cost = D[i][k] + D[k + 1][j];
		height = std::max(H[i][k], H[k + 1][j]);

		if (cost == D[i][j] && height == H[i][j]) {
			rebuild(n, D, H, i, k); /* Rebuild first half */
			rebuild(n, D, H, k + 1, j); /* Rebuild second half */
			return;
		}
	}
}
