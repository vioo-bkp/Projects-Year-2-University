#include <cstdio>
#include <cstring>
#define NMAX 359 /* Maximum size for input */
#define ALPHA 26 /* Size of alphabet */
#define ord(c) (c - 'a') /* Index of a letter in the alphabet */

/* solve the problem for a particular string s */
int solve(char *s);

int main() {
	/* open the files */
	freopen("joc.in", "r", stdin);
	freopen("joc.out", "w", stdout);

	int T; /* number of tests */
	scanf("%d", &T);

	char s[NMAX]; /* input string for one test */

	for (int i = 0; i < T; ++i) {
		/* solve each test and print result */
		scanf("%s", s);
		printf("%d\n", solve(s));
	}

	return 0;
}

/* solve the problem for a particular string s */
int solve(char *s) {
	/* Define freq[ i ] = frequency of ith letter from alphabet
						  in string s 	
	   E.g. freq[ 0 ] = number of occurences of letter 'a' in s */
	int freq[ALPHA + 1] = { 0 };
	char *i, *j, *k; /* pointers from buffer s */
	int sol = 0; /* the solution */

	/* compute frequency for each letter */
	for (i  = s ; *i; ++i) {
		++freq[ ord(*i) ];
	}

	/* The problem has solution if and only if there is 
	one (ore none) letter with odd frequency */ 
	int countOdd = 0;
	for (int i  = 0 ; i < ALPHA; ++i) {
		/* check if this letter has odd frequency */
		if (freq[i] & 1) {
			++countOdd;
			if (countOdd > 1) {
				return -1; /* Too much! No solution */
			}
		}
	}
	/* The problem has solution! */

	i = s; /* pointer to first character */
	j = s + strlen(s) - 1; /* pointer to last character */
	while (i < j) {
		/* Check if i and j don't match */
		if (*i != *j) {
			/* Check if *i is the character from the middle */
			if (freq[ ord(*i) ] == 1) {
				/* Move it to the middle*/
				char c = *i;
				*i = *(i + 1);
				*(i + 1) = c;
				++sol;
			}

			/* Check if *j is the character from the middle */
			if (freq[ ord(*j) ] == 1) {
				/* Move it to the middle*/
				char c = *j;
				*j = *(j - 1);
				*(j - 1) = c;
				++sol;
			}

			/* Check if i and j don't match */
			if (*i != *j) {
				/* other_i pointer to first character from (i, j]
				   that equals to *i (left -> right)

				   other_j pointer to first character from [i, j)
				   that equals to *j (right -> left)*/
				char *other_i, *other_j;

				/* Find a character other_i */
				for (other_i = i + 1; other_i < j; ++other_i) {
					if (*other_i == *j) {
						break;
					}
				}

				//* Find a character other_j */
				for (other_j = j - 1; other_j > i; --other_j) {
					if (*other_j == *i) {
						break;
					}
				}

				int sol_other_i = other_i - i;
				int sol_other_j = j-other_j;
				if ( sol_other_j < sol_other_i ) {
					/* shift character from k to j to the left */
					for (k = other_j; k < j; ++k) {
						*k = *(k + 1);
						++sol;
					}
					*j = *i;
				} else {
					/* shift character from i to k to the right */
					for (k = other_i; k > i; --k) {
						*k = *(k - 1);
						++sol;
					}
					*i = *j;
				}
			}
		}

		/* *i matches *j */
		freq[ ord(*i) ] -= 2;

		/* get to next pair */
		++i;
		--j;
	}

    return sol;
}
